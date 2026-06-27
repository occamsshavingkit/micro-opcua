/* src/address_space/node_id.c */
#include "micro_opcua/address_space.h"
#include <string.h>

#undef mu_address_space_find_node

typedef struct {
    opcua_uint16_t namespace_index;
    mu_nodeid_type_t identifier_type;
    opcua_uint32_t identifier_key;
} mu_nodeid_sort_key_t;

static opcua_uint32_t mu_nodeid_hash_mix(opcua_uint32_t hash, opcua_uint32_t value) {
    hash ^= value;
    return hash * 16777619u;
}

static opcua_uint32_t mu_nodeid_string_hash(const mu_string_t *value) {
    opcua_uint32_t hash = 2166136261u;
    opcua_uint32_t length;
    size_t i;

    if (!value) {
        return hash;
    }

    length = (opcua_uint32_t)value->length;
    hash = mu_nodeid_hash_mix(hash, length & 0xffu);
    hash = mu_nodeid_hash_mix(hash, (length >> 8) & 0xffu);
    hash = mu_nodeid_hash_mix(hash, (length >> 16) & 0xffu);
    hash = mu_nodeid_hash_mix(hash, (length >> 24) & 0xffu);

    if (value->length <= 0 || !value->data) {
        return hash;
    }

    for (i = 0; i < (size_t)value->length; i++) {
        hash = mu_nodeid_hash_mix(hash, (opcua_uint32_t)value->data[i]);
    }

    return hash;
}

static mu_nodeid_sort_key_t mu_nodeid_sort_key(const mu_nodeid_t *node_id) {
    mu_nodeid_sort_key_t key;

    key.namespace_index = 0;
    key.identifier_type = MU_NODEID_NUMERIC;
    key.identifier_key = 0;

    if (!node_id) {
        return key;
    }

    key.namespace_index = node_id->namespace_index;
    key.identifier_type = node_id->identifier_type;

    switch (node_id->identifier_type) {
        case MU_NODEID_NUMERIC:
            key.identifier_key = node_id->identifier.numeric;
            break;
        case MU_NODEID_STRING:
            key.identifier_key = mu_nodeid_string_hash(&node_id->identifier.string);
            break;
        default:
            key.identifier_key = 0;
            break;
    }

    return key;
}

static int mu_nodeid_sort_key_compare(const mu_nodeid_sort_key_t *left, const mu_nodeid_sort_key_t *right) {
    if (left->namespace_index < right->namespace_index) {
        return -1;
    }
    if (left->namespace_index > right->namespace_index) {
        return 1;
    }

    if (left->identifier_type < right->identifier_type) {
        return -1;
    }
    if (left->identifier_type > right->identifier_type) {
        return 1;
    }

    if (left->identifier_key < right->identifier_key) {
        return -1;
    }
    if (left->identifier_key > right->identifier_key) {
        return 1;
    }

    return 0;
}

static int mu_nodeid_compare_for_index(const mu_nodeid_t *left, const mu_nodeid_t *right) {
    mu_nodeid_sort_key_t left_key = mu_nodeid_sort_key(left);
    mu_nodeid_sort_key_t right_key = mu_nodeid_sort_key(right);
    return mu_nodeid_sort_key_compare(&left_key, &right_key);
}

static int mu_nodeid_compare_to_key(const mu_nodeid_t *node_id, const mu_nodeid_sort_key_t *key) {
    mu_nodeid_sort_key_t node_key = mu_nodeid_sort_key(node_id);
    return mu_nodeid_sort_key_compare(&node_key, key);
}

static void mu_address_space_rebuild_index(const mu_address_space_t *address_space,
                                           mu_address_space_index_t *index) {
    size_t i;

    index->built_for = address_space;
    index->built_count = address_space->node_count;
    index->count = 0;
    index->indexed = false;

    if (address_space->node_count > MU_MAX_ADDRESS_SPACE_NODES || address_space->node_count > 0xffffu) {
        return;
    }

    index->indexed = true;

    for (i = 0; i < address_space->node_count; i++) {
        size_t position = index->count;

        while (position > 0) {
            const mu_node_t *previous = &address_space->nodes[index->order[position - 1u]];
            const mu_node_t *current = &address_space->nodes[i];

            if (mu_nodeid_compare_for_index(&previous->node_id, &current->node_id) <= 0) {
                break;
            }

            index->order[position] = index->order[position - 1u];
            position--;
        }

        index->order[position] = (opcua_uint16_t)i;
        index->count++;
    }
}

static const mu_node_t *mu_address_space_find_node_linear(const mu_address_space_t *address_space,
                                                          const mu_nodeid_t *node_id) {
    size_t i;

    for (i = 0; i < address_space->node_count; i++) {
        if (mu_nodeid_equal(&address_space->nodes[i].node_id, node_id)) {
            return &address_space->nodes[i];
        }
    }

    return NULL;
}

static const mu_node_t *mu_address_space_find_node_indexed(const mu_address_space_t *address_space,
                                                           const mu_address_space_index_t *index,
                                                           const mu_nodeid_t *node_id) {
    mu_nodeid_sort_key_t target_key = mu_nodeid_sort_key(node_id);
    size_t left = 0;
    size_t right = index->count;

    while (left < right) {
        size_t mid = left + ((right - left) / 2u);
        opcua_uint16_t node_index = index->order[mid];
        const mu_node_t *node = &address_space->nodes[node_index];
        int comparison = mu_nodeid_compare_to_key(&node->node_id, &target_key);

        if (comparison < 0) {
            left = mid + 1u;
        } else if (comparison > 0) {
            right = mid;
        } else {
            size_t position = mid;

            while (position > 0) {
                opcua_uint16_t previous_index = index->order[position - 1u];
                const mu_node_t *previous = &address_space->nodes[previous_index];

                if (mu_nodeid_compare_to_key(&previous->node_id, &target_key) != 0) {
                    break;
                }
                position--;
            }

            while (position < index->count) {
                opcua_uint16_t candidate_index = index->order[position];
                const mu_node_t *candidate = &address_space->nodes[candidate_index];

                if (mu_nodeid_compare_to_key(&candidate->node_id, &target_key) != 0) {
                    break;
                }

                if (mu_nodeid_equal(&candidate->node_id, node_id)) {
                    return candidate;
                }

                position++;
            }

            return NULL;
        }
    }

    return NULL;
}

opcua_boolean_t mu_nodeid_equal(const mu_nodeid_t *n1, const mu_nodeid_t *n2) {
    if (!n1 || !n2) {
        return false;
    }
    
    if (n1->namespace_index != n2->namespace_index) {
        return false;
    }
    
    if (n1->identifier_type != n2->identifier_type) {
        return false;
    }
    
    switch (n1->identifier_type) {
        case MU_NODEID_NUMERIC:
            return n1->identifier.numeric == n2->identifier.numeric;
            
        case MU_NODEID_STRING:
            if (n1->identifier.string.length != n2->identifier.string.length) {
                return false;
            }
            if (n1->identifier.string.length <= 0) {
                return true;
            }
            if (!n1->identifier.string.data || !n2->identifier.string.data) {
                return n1->identifier.string.data == n2->identifier.string.data;
            }
            return memcmp(n1->identifier.string.data, n2->identifier.string.data, (size_t)n1->identifier.string.length) == 0;
            
        default:
            /* GUID and Opaque not supported in minimal profile */
            return false;
    }
}

opcua_boolean_t mu_nodeid_in_namespace(const mu_nodeid_t *node_id, opcua_uint16_t namespace_index) {
    if (!node_id) {
        return false;
    }
    return node_id->namespace_index == namespace_index;
}

const mu_node_t *mu_address_space_find_node(const mu_address_space_t *address_space,
                                            mu_address_space_index_t *index,
                                            const mu_nodeid_t *node_id) {
    if (!address_space || !address_space->nodes || !node_id) {
        return NULL;
    }

    if (index == NULL) {
        return mu_address_space_find_node_linear(address_space, node_id);
    }

    if (index->built_for != address_space || index->built_count != address_space->node_count) {
        mu_address_space_rebuild_index(address_space, index);
    }

    if (!index->indexed) {
        return mu_address_space_find_node_linear(address_space, node_id);
    }

    return mu_address_space_find_node_indexed(address_space, index, node_id);
}
