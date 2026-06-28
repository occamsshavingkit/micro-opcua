/* src/address_space/base_nodes.c */
#include "base_nodes.h"
#include <stddef.h>

#ifdef MICRO_OPCUA_BASE_NODES
static const mu_reference_t s_root_refs[] = {{{0, MU_NODEID_NUMERIC, {35}}, {0, MU_NODEID_NUMERIC, {85}}, true},
                                             {{0, MU_NODEID_NUMERIC, {35}}, {0, MU_NODEID_NUMERIC, {86}}, true},
                                             {{0, MU_NODEID_NUMERIC, {35}}, {0, MU_NODEID_NUMERIC, {87}}, true},
#if MICRO_OPCUA_BASE_TYPE_SYSTEM
                                             {{0, MU_NODEID_NUMERIC, {40}}, {0, MU_NODEID_NUMERIC, {61}}, true}
#endif
};

static const mu_reference_t s_objects_refs[] = {{{0, MU_NODEID_NUMERIC, {35}}, {0, MU_NODEID_NUMERIC, {2253}}, true},
#if MICRO_OPCUA_BASE_TYPE_SYSTEM
                                                {{0, MU_NODEID_NUMERIC, {40}}, {0, MU_NODEID_NUMERIC, {61}}, true}
#endif
};

#if MICRO_OPCUA_BASE_TYPE_SYSTEM
static const mu_reference_t s_views_refs[] = {{{0, MU_NODEID_NUMERIC, {40}}, {0, MU_NODEID_NUMERIC, {61}}, true}};
#endif

#if MICRO_OPCUA_BASE_TYPE_SYSTEM
static const mu_reference_t s_types_refs[] = {{{0, MU_NODEID_NUMERIC, {35}}, {0, MU_NODEID_NUMERIC, {88}}, true},
                                              {{0, MU_NODEID_NUMERIC, {35}}, {0, MU_NODEID_NUMERIC, {89}}, true},
                                              {{0, MU_NODEID_NUMERIC, {35}}, {0, MU_NODEID_NUMERIC, {90}}, true},
                                              {{0, MU_NODEID_NUMERIC, {35}}, {0, MU_NODEID_NUMERIC, {91}}, true},
                                              {{0, MU_NODEID_NUMERIC, {40}}, {0, MU_NODEID_NUMERIC, {61}}, true}};

static const mu_reference_t s_type_folder_object_refs[] = {
    {{0, MU_NODEID_NUMERIC, {35}}, {0, MU_NODEID_NUMERIC, {58}}, true},
    {{0, MU_NODEID_NUMERIC, {40}}, {0, MU_NODEID_NUMERIC, {61}}, true}};

static const mu_reference_t s_type_folder_variable_refs[] = {
    {{0, MU_NODEID_NUMERIC, {35}}, {0, MU_NODEID_NUMERIC, {62}}, true},
    {{0, MU_NODEID_NUMERIC, {40}}, {0, MU_NODEID_NUMERIC, {61}}, true}};

static const mu_reference_t s_type_folder_data_refs[] = {
    {{0, MU_NODEID_NUMERIC, {35}}, {0, MU_NODEID_NUMERIC, {24}}, true},
    {{0, MU_NODEID_NUMERIC, {40}}, {0, MU_NODEID_NUMERIC, {61}}, true}};

static const mu_reference_t s_type_folder_reference_refs[] = {
    {{0, MU_NODEID_NUMERIC, {35}}, {0, MU_NODEID_NUMERIC, {31}}, true},
    {{0, MU_NODEID_NUMERIC, {40}}, {0, MU_NODEID_NUMERIC, {61}}, true}};
#endif

#if MICRO_OPCUA_BASE_TYPE_SYSTEM
static const mu_reference_t s_references_refs[] = {{{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {32}}, true},
                                                   {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {33}}, true}};

static const mu_reference_t s_nonhierarchical_refs[] = {
    {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {40}}, true}};

static const mu_reference_t s_hierarchical_refs[] = {
    {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {34}}, true},
    {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {35}}, true}};

static const mu_reference_t s_has_child_refs[] = {{{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {44}}, true},
                                                  {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {45}}, true}};

static const mu_reference_t s_aggregates_refs[] = {{{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {46}}, true},
                                                   {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {47}}, true}};

static const mu_reference_t s_base_data_type_refs[] = {
    {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {1}}, true},
    {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {2}}, true},
    {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {3}}, true},
    {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {4}}, true},
    {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {5}}, true},
    {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {6}}, true},
    {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {7}}, true},
    {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {8}}, true},
    {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {9}}, true},
    {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {10}}, true},
    {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {11}}, true},
    {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {12}}, true},
    {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {13}}, true},
    {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {17}}, true},
    {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {19}}, true},
    {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {20}}, true},
    {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {21}}, true}};

static const mu_reference_t s_base_object_type_refs[] = {
    {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {61}}, true},
    {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {2004}}, true}};

static const mu_reference_t s_base_variable_type_refs[] = {
    {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {63}}, true}};

static const mu_reference_t s_base_data_variable_type_refs[] = {
    {{0, MU_NODEID_NUMERIC, {45}}, {0, MU_NODEID_NUMERIC, {68}}, true}};
#endif

static const mu_reference_t s_server_refs[] = {{{0, MU_NODEID_NUMERIC, {47}}, {0, MU_NODEID_NUMERIC, {2254}}, true},
                                               {{0, MU_NODEID_NUMERIC, {47}}, {0, MU_NODEID_NUMERIC, {2255}}, true},
                                               {{0, MU_NODEID_NUMERIC, {47}}, {0, MU_NODEID_NUMERIC, {2256}}, true},
                                               {{0, MU_NODEID_NUMERIC, {47}}, {0, MU_NODEID_NUMERIC, {2268}}, true},
#if MICRO_OPCUA_BASE_TYPE_SYSTEM && MICRO_OPCUA_SUBSCRIPTIONS_STANDARD
                                               {{0, MU_NODEID_NUMERIC, {47}}, {0, MU_NODEID_NUMERIC, {11492}}, true},
                                               {{0, MU_NODEID_NUMERIC, {47}}, {0, MU_NODEID_NUMERIC, {12873}}, true},
#endif
#if MICRO_OPCUA_BASE_TYPE_SYSTEM
                                               {{0, MU_NODEID_NUMERIC, {40}}, {0, MU_NODEID_NUMERIC, {2004}}, true}
#endif
};

static const mu_reference_t s_server_status_refs[] = {
    {{0, MU_NODEID_NUMERIC, {47}}, {0, MU_NODEID_NUMERIC, {2259}}, true},
    {{0, MU_NODEID_NUMERIC, {47}}, {0, MU_NODEID_NUMERIC, {2258}}, true},
    {{0, MU_NODEID_NUMERIC, {47}}, {0, MU_NODEID_NUMERIC, {2257}}, true},
#if MICRO_OPCUA_BASE_TYPE_SYSTEM
    {{0, MU_NODEID_NUMERIC, {40}}, {0, MU_NODEID_NUMERIC, {63}}, true}
#endif
};

static const mu_reference_t s_server_capabilities_refs[] = {
    {{0, MU_NODEID_NUMERIC, {46}}, {0, MU_NODEID_NUMERIC, {2269}}, true},
    {{0, MU_NODEID_NUMERIC, {46}}, {0, MU_NODEID_NUMERIC, {2271}}, true},
    {{0, MU_NODEID_NUMERIC, {47}}, {0, MU_NODEID_NUMERIC, {11704}}, true},
#if MICRO_OPCUA_BASE_TYPE_SYSTEM
    {{0, MU_NODEID_NUMERIC, {40}}, {0, MU_NODEID_NUMERIC, {58}}, true}
#endif
};

static const mu_reference_t s_operation_limits_refs[] = {
    {{0, MU_NODEID_NUMERIC, {46}}, {0, MU_NODEID_NUMERIC, {11705}}, true},
    {{0, MU_NODEID_NUMERIC, {46}}, {0, MU_NODEID_NUMERIC, {11710}}, true},
#if MICRO_OPCUA_BASE_TYPE_SYSTEM
    {{0, MU_NODEID_NUMERIC, {40}}, {0, MU_NODEID_NUMERIC, {58}}, true}
#endif
};

#if MICRO_OPCUA_BASE_TYPE_SYSTEM
static const mu_reference_t s_property_type_ref[] = {
    {{0, MU_NODEID_NUMERIC, {40}}, {0, MU_NODEID_NUMERIC, {68}}, true}};

#if MICRO_OPCUA_SUBSCRIPTIONS_STANDARD
static const mu_reference_t s_get_monitored_items_refs[] = {
    {{0, MU_NODEID_NUMERIC, {46}}, {0, MU_NODEID_NUMERIC, {11493}}, true},
    {{0, MU_NODEID_NUMERIC, {46}}, {0, MU_NODEID_NUMERIC, {11494}}, true}};

static const mu_reference_t s_resend_data_refs[] = {
    {{0, MU_NODEID_NUMERIC, {46}}, {0, MU_NODEID_NUMERIC, {12874}}, true}};
#endif

#endif

static const mu_string_t s_server_array[] = {{22, (const opcua_byte_t *)"urn:micro-opcua:server"}};

static const mu_value_source_t s_server_array_value = {
    MU_VALUESOURCE_STATIC,
    {.static_value = {.type = MU_TYPE_STRING, .value.array = s_server_array, .is_array = true, .array_length = 1}}};

static const mu_string_t s_namespace_array[] = {{28, (const opcua_byte_t *)"http://opcfoundation.org/UA/"}};

static const mu_value_source_t s_namespace_array_value = {
    MU_VALUESOURCE_STATIC,
    {.static_value = {.type = MU_TYPE_STRING, .value.array = s_namespace_array, .is_array = true, .array_length = 1}}};

static const mu_value_source_t s_server_status_state_value = {
    MU_VALUESOURCE_STATIC, {.static_value = {.type = MU_TYPE_INT32, .value.i32 = 0}}};

static const mu_string_t s_server_profile_array[] = {
#if MICRO_OPCUA_BASE_TYPE_SYSTEM
    {57, (const opcua_byte_t *)"http://opcfoundation.org/UA-Profile/Server/EmbeddedUA2017"}
#else
    {65, (const opcua_byte_t *)"http://opcfoundation.org/UA-Profile/Server/NanoEmbeddedDevice2017"}
#endif
};

static const mu_value_source_t s_server_profile_array_value = {
    MU_VALUESOURCE_STATIC,
    {.static_value = {
         .type = MU_TYPE_STRING, .value.array = s_server_profile_array, .is_array = true, .array_length = 1}}};

static const mu_string_t s_locale_id_array[] = {{2, (const opcua_byte_t *)"en"}};

static const mu_value_source_t s_locale_id_array_value = {
    MU_VALUESOURCE_STATIC,
    {.static_value = {.type = MU_TYPE_STRING, .value.array = s_locale_id_array, .is_array = true, .array_length = 1}}};

static const mu_value_source_t s_max_nodes_per_read_value = {
    MU_VALUESOURCE_STATIC, {.static_value = {.type = MU_TYPE_UINT32, .value.ui32 = 32}}};

static const mu_value_source_t s_max_nodes_per_browse_value = {
    MU_VALUESOURCE_STATIC, {.static_value = {.type = MU_TYPE_UINT32, .value.ui32 = 8}}};

static const mu_node_t s_base_nodes[] = {
    {{0, MU_NODEID_NUMERIC, {84}},
     MU_NODECLASS_OBJECT,
     {4, (const opcua_byte_t *)"Root"},
     {4, (const opcua_byte_t *)"Root"},
     s_root_refs,
     sizeof(s_root_refs) / sizeof(s_root_refs[0]),
     NULL},
    {{0, MU_NODEID_NUMERIC, {85}},
     MU_NODECLASS_OBJECT,
     {7, (const opcua_byte_t *)"Objects"},
     {7, (const opcua_byte_t *)"Objects"},
     s_objects_refs,
     sizeof(s_objects_refs) / sizeof(s_objects_refs[0]),
     NULL},
    {{0, MU_NODEID_NUMERIC, {86}},
     MU_NODECLASS_OBJECT,
     {5, (const opcua_byte_t *)"Types"},
     {5, (const opcua_byte_t *)"Types"},
#if MICRO_OPCUA_BASE_TYPE_SYSTEM
     s_types_refs,
     sizeof(s_types_refs) / sizeof(s_types_refs[0]),
#else
     NULL,
     0,
#endif
     NULL},
    {{0, MU_NODEID_NUMERIC, {87}},
     MU_NODECLASS_OBJECT,
     {5, (const opcua_byte_t *)"Views"},
     {5, (const opcua_byte_t *)"Views"},
#if MICRO_OPCUA_BASE_TYPE_SYSTEM
     s_views_refs,
     sizeof(s_views_refs) / sizeof(s_views_refs[0]),
#else
     NULL,
     0,
#endif
     NULL},
    {{0, MU_NODEID_NUMERIC, {2253}},
     MU_NODECLASS_OBJECT,
     {6, (const opcua_byte_t *)"Server"},
     {6, (const opcua_byte_t *)"Server"},
     s_server_refs,
     sizeof(s_server_refs) / sizeof(s_server_refs[0]),
     NULL},
    {{0, MU_NODEID_NUMERIC, {2254}},
     MU_NODECLASS_VARIABLE,
     {11, (const opcua_byte_t *)"ServerArray"},
     {11, (const opcua_byte_t *)"ServerArray"},
#if MICRO_OPCUA_BASE_TYPE_SYSTEM
     s_property_type_ref,
     sizeof(s_property_type_ref) / sizeof(s_property_type_ref[0]),
#else
     NULL,
     0,
#endif
     &s_server_array_value},
    {{0, MU_NODEID_NUMERIC, {2255}},
     MU_NODECLASS_VARIABLE,
     {14, (const opcua_byte_t *)"NamespaceArray"},
     {14, (const opcua_byte_t *)"NamespaceArray"},
#if MICRO_OPCUA_BASE_TYPE_SYSTEM
     s_property_type_ref,
     sizeof(s_property_type_ref) / sizeof(s_property_type_ref[0]),
#else
     NULL,
     0,
#endif
     &s_namespace_array_value},
    {{0, MU_NODEID_NUMERIC, {2256}},
     MU_NODECLASS_VARIABLE,
     {12, (const opcua_byte_t *)"ServerStatus"},
     {12, (const opcua_byte_t *)"ServerStatus"},
     s_server_status_refs,
     sizeof(s_server_status_refs) / sizeof(s_server_status_refs[0]),
     NULL},
    {{0, MU_NODEID_NUMERIC, {2259}},
     MU_NODECLASS_VARIABLE,
     {5, (const opcua_byte_t *)"State"},
     {5, (const opcua_byte_t *)"State"},
#if MICRO_OPCUA_BASE_TYPE_SYSTEM
     s_property_type_ref,
     sizeof(s_property_type_ref) / sizeof(s_property_type_ref[0]),
#else
     NULL,
     0,
#endif
     &s_server_status_state_value},
    {{0, MU_NODEID_NUMERIC, {2268}},
     MU_NODECLASS_OBJECT,
     {18, (const opcua_byte_t *)"ServerCapabilities"},
     {18, (const opcua_byte_t *)"ServerCapabilities"},
     s_server_capabilities_refs,
     sizeof(s_server_capabilities_refs) / sizeof(s_server_capabilities_refs[0]),
     NULL},
    {{0, MU_NODEID_NUMERIC, {2269}},
     MU_NODECLASS_VARIABLE,
     {18, (const opcua_byte_t *)"ServerProfileArray"},
     {18, (const opcua_byte_t *)"ServerProfileArray"},
#if MICRO_OPCUA_BASE_TYPE_SYSTEM
     s_property_type_ref,
     sizeof(s_property_type_ref) / sizeof(s_property_type_ref[0]),
#else
     NULL,
     0,
#endif
     &s_server_profile_array_value},
    {{0, MU_NODEID_NUMERIC, {2271}},
     MU_NODECLASS_VARIABLE,
     {13, (const opcua_byte_t *)"LocaleIdArray"},
     {13, (const opcua_byte_t *)"LocaleIdArray"},
#if MICRO_OPCUA_BASE_TYPE_SYSTEM
     s_property_type_ref,
     sizeof(s_property_type_ref) / sizeof(s_property_type_ref[0]),
#else
     NULL,
     0,
#endif
     &s_locale_id_array_value},
    {{0, MU_NODEID_NUMERIC, {11704}},
     MU_NODECLASS_OBJECT,
     {15, (const opcua_byte_t *)"OperationLimits"},
     {15, (const opcua_byte_t *)"OperationLimits"},
     s_operation_limits_refs,
     sizeof(s_operation_limits_refs) / sizeof(s_operation_limits_refs[0]),
     NULL},
    {{0, MU_NODEID_NUMERIC, {11705}},
     MU_NODECLASS_VARIABLE,
     {15, (const opcua_byte_t *)"MaxNodesPerRead"},
     {15, (const opcua_byte_t *)"MaxNodesPerRead"},
#if MICRO_OPCUA_BASE_TYPE_SYSTEM
     s_property_type_ref,
     sizeof(s_property_type_ref) / sizeof(s_property_type_ref[0]),
#else
     NULL,
     0,
#endif
     &s_max_nodes_per_read_value},
    {{0, MU_NODEID_NUMERIC, {11710}},
     MU_NODECLASS_VARIABLE,
     {17, (const opcua_byte_t *)"MaxNodesPerBrowse"},
     {17, (const opcua_byte_t *)"MaxNodesPerBrowse"},
#if MICRO_OPCUA_BASE_TYPE_SYSTEM
     s_property_type_ref,
     sizeof(s_property_type_ref) / sizeof(s_property_type_ref[0]),
#else
     NULL,
     0,
#endif
     &s_max_nodes_per_browse_value},
#if MICRO_OPCUA_BASE_TYPE_SYSTEM
    {{0, MU_NODEID_NUMERIC, {88}},
     MU_NODECLASS_OBJECT,
     {11, (const opcua_byte_t *)"ObjectTypes"},
     {11, (const opcua_byte_t *)"ObjectTypes"},
     s_type_folder_object_refs,
     sizeof(s_type_folder_object_refs) / sizeof(s_type_folder_object_refs[0]),
     NULL},
    {{0, MU_NODEID_NUMERIC, {89}},
     MU_NODECLASS_OBJECT,
     {13, (const opcua_byte_t *)"VariableTypes"},
     {13, (const opcua_byte_t *)"VariableTypes"},
     s_type_folder_variable_refs,
     sizeof(s_type_folder_variable_refs) / sizeof(s_type_folder_variable_refs[0]),
     NULL},
    {{0, MU_NODEID_NUMERIC, {90}},
     MU_NODECLASS_OBJECT,
     {9, (const opcua_byte_t *)"DataTypes"},
     {9, (const opcua_byte_t *)"DataTypes"},
     s_type_folder_data_refs,
     sizeof(s_type_folder_data_refs) / sizeof(s_type_folder_data_refs[0]),
     NULL},
    {{0, MU_NODEID_NUMERIC, {91}},
     MU_NODECLASS_OBJECT,
     {14, (const opcua_byte_t *)"ReferenceTypes"},
     {14, (const opcua_byte_t *)"ReferenceTypes"},
     s_type_folder_reference_refs,
     sizeof(s_type_folder_reference_refs) / sizeof(s_type_folder_reference_refs[0]),
     NULL},

    {{0, MU_NODEID_NUMERIC, {31}},
     MU_NODECLASS_REFERENCETYPE,
     {10, (const opcua_byte_t *)"References"},
     {10, (const opcua_byte_t *)"References"},
     s_references_refs,
     sizeof(s_references_refs) / sizeof(s_references_refs[0]),
     NULL},
    {{0, MU_NODEID_NUMERIC, {32}},
     MU_NODECLASS_REFERENCETYPE,
     {25, (const opcua_byte_t *)"NonHierarchicalReferences"},
     {25, (const opcua_byte_t *)"NonHierarchicalReferences"},
     s_nonhierarchical_refs,
     sizeof(s_nonhierarchical_refs) / sizeof(s_nonhierarchical_refs[0]),
     NULL},
    {{0, MU_NODEID_NUMERIC, {33}},
     MU_NODECLASS_REFERENCETYPE,
     {22, (const opcua_byte_t *)"HierarchicalReferences"},
     {22, (const opcua_byte_t *)"HierarchicalReferences"},
     s_hierarchical_refs,
     sizeof(s_hierarchical_refs) / sizeof(s_hierarchical_refs[0]),
     NULL},
    {{0, MU_NODEID_NUMERIC, {34}},
     MU_NODECLASS_REFERENCETYPE,
     {8, (const opcua_byte_t *)"HasChild"},
     {8, (const opcua_byte_t *)"HasChild"},
     s_has_child_refs,
     sizeof(s_has_child_refs) / sizeof(s_has_child_refs[0]),
     NULL},
    {{0, MU_NODEID_NUMERIC, {35}},
     MU_NODECLASS_REFERENCETYPE,
     {9, (const opcua_byte_t *)"Organizes"},
     {9, (const opcua_byte_t *)"Organizes"},
     NULL,
     0,
     NULL},
    {{0, MU_NODEID_NUMERIC, {40}},
     MU_NODECLASS_REFERENCETYPE,
     {17, (const opcua_byte_t *)"HasTypeDefinition"},
     {17, (const opcua_byte_t *)"HasTypeDefinition"},
     NULL,
     0,
     NULL},
    {{0, MU_NODEID_NUMERIC, {44}},
     MU_NODECLASS_REFERENCETYPE,
     {10, (const opcua_byte_t *)"Aggregates"},
     {10, (const opcua_byte_t *)"Aggregates"},
     s_aggregates_refs,
     sizeof(s_aggregates_refs) / sizeof(s_aggregates_refs[0]),
     NULL},
    {{0, MU_NODEID_NUMERIC, {45}},
     MU_NODECLASS_REFERENCETYPE,
     {10, (const opcua_byte_t *)"HasSubtype"},
     {10, (const opcua_byte_t *)"HasSubtype"},
     NULL,
     0,
     NULL},
    {{0, MU_NODEID_NUMERIC, {46}},
     MU_NODECLASS_REFERENCETYPE,
     {11, (const opcua_byte_t *)"HasProperty"},
     {11, (const opcua_byte_t *)"HasProperty"},
     NULL,
     0,
     NULL},
    {{0, MU_NODEID_NUMERIC, {47}},
     MU_NODECLASS_REFERENCETYPE,
     {12, (const opcua_byte_t *)"HasComponent"},
     {12, (const opcua_byte_t *)"HasComponent"},
     NULL,
     0,
     NULL},

    {{0, MU_NODEID_NUMERIC, {24}},
     MU_NODECLASS_DATATYPE,
     {12, (const opcua_byte_t *)"BaseDataType"},
     {12, (const opcua_byte_t *)"BaseDataType"},
     s_base_data_type_refs,
     sizeof(s_base_data_type_refs) / sizeof(s_base_data_type_refs[0]),
     NULL},
    {{0, MU_NODEID_NUMERIC, {1}},
     MU_NODECLASS_DATATYPE,
     {7, (const opcua_byte_t *)"Boolean"},
     {7, (const opcua_byte_t *)"Boolean"},
     NULL,
     0,
     NULL},
    {{0, MU_NODEID_NUMERIC, {2}},
     MU_NODECLASS_DATATYPE,
     {5, (const opcua_byte_t *)"SByte"},
     {5, (const opcua_byte_t *)"SByte"},
     NULL,
     0,
     NULL},
    {{0, MU_NODEID_NUMERIC, {3}},
     MU_NODECLASS_DATATYPE,
     {4, (const opcua_byte_t *)"Byte"},
     {4, (const opcua_byte_t *)"Byte"},
     NULL,
     0,
     NULL},
    {{0, MU_NODEID_NUMERIC, {4}},
     MU_NODECLASS_DATATYPE,
     {5, (const opcua_byte_t *)"Int16"},
     {5, (const opcua_byte_t *)"Int16"},
     NULL,
     0,
     NULL},
    {{0, MU_NODEID_NUMERIC, {5}},
     MU_NODECLASS_DATATYPE,
     {6, (const opcua_byte_t *)"UInt16"},
     {6, (const opcua_byte_t *)"UInt16"},
     NULL,
     0,
     NULL},
    {{0, MU_NODEID_NUMERIC, {6}},
     MU_NODECLASS_DATATYPE,
     {5, (const opcua_byte_t *)"Int32"},
     {5, (const opcua_byte_t *)"Int32"},
     NULL,
     0,
     NULL},
    {{0, MU_NODEID_NUMERIC, {7}},
     MU_NODECLASS_DATATYPE,
     {6, (const opcua_byte_t *)"UInt32"},
     {6, (const opcua_byte_t *)"UInt32"},
     NULL,
     0,
     NULL},
    {{0, MU_NODEID_NUMERIC, {8}},
     MU_NODECLASS_DATATYPE,
     {5, (const opcua_byte_t *)"Int64"},
     {5, (const opcua_byte_t *)"Int64"},
     NULL,
     0,
     NULL},
    {{0, MU_NODEID_NUMERIC, {9}},
     MU_NODECLASS_DATATYPE,
     {6, (const opcua_byte_t *)"UInt64"},
     {6, (const opcua_byte_t *)"UInt64"},
     NULL,
     0,
     NULL},
    {{0, MU_NODEID_NUMERIC, {10}},
     MU_NODECLASS_DATATYPE,
     {5, (const opcua_byte_t *)"Float"},
     {5, (const opcua_byte_t *)"Float"},
     NULL,
     0,
     NULL},
    {{0, MU_NODEID_NUMERIC, {11}},
     MU_NODECLASS_DATATYPE,
     {6, (const opcua_byte_t *)"Double"},
     {6, (const opcua_byte_t *)"Double"},
     NULL,
     0,
     NULL},
    {{0, MU_NODEID_NUMERIC, {12}},
     MU_NODECLASS_DATATYPE,
     {6, (const opcua_byte_t *)"String"},
     {6, (const opcua_byte_t *)"String"},
     NULL,
     0,
     NULL},
    {{0, MU_NODEID_NUMERIC, {13}},
     MU_NODECLASS_DATATYPE,
     {8, (const opcua_byte_t *)"DateTime"},
     {8, (const opcua_byte_t *)"DateTime"},
     NULL,
     0,
     NULL},
    {{0, MU_NODEID_NUMERIC, {17}},
     MU_NODECLASS_DATATYPE,
     {6, (const opcua_byte_t *)"NodeId"},
     {6, (const opcua_byte_t *)"NodeId"},
     NULL,
     0,
     NULL},
    {{0, MU_NODEID_NUMERIC, {19}},
     MU_NODECLASS_DATATYPE,
     {10, (const opcua_byte_t *)"StatusCode"},
     {10, (const opcua_byte_t *)"StatusCode"},
     NULL,
     0,
     NULL},
    {{0, MU_NODEID_NUMERIC, {20}},
     MU_NODECLASS_DATATYPE,
     {13, (const opcua_byte_t *)"QualifiedName"},
     {13, (const opcua_byte_t *)"QualifiedName"},
     NULL,
     0,
     NULL},
    {{0, MU_NODEID_NUMERIC, {21}},
     MU_NODECLASS_DATATYPE,
     {13, (const opcua_byte_t *)"LocalizedText"},
     {13, (const opcua_byte_t *)"LocalizedText"},
     NULL,
     0,
     NULL},

    {{0, MU_NODEID_NUMERIC, {58}},
     MU_NODECLASS_OBJECTTYPE,
     {14, (const opcua_byte_t *)"BaseObjectType"},
     {14, (const opcua_byte_t *)"BaseObjectType"},
     s_base_object_type_refs,
     sizeof(s_base_object_type_refs) / sizeof(s_base_object_type_refs[0]),
     NULL},
    {{0, MU_NODEID_NUMERIC, {61}},
     MU_NODECLASS_OBJECTTYPE,
     {10, (const opcua_byte_t *)"FolderType"},
     {10, (const opcua_byte_t *)"FolderType"},
     NULL,
     0,
     NULL},
    {{0, MU_NODEID_NUMERIC, {2004}},
     MU_NODECLASS_OBJECTTYPE,
     {10, (const opcua_byte_t *)"ServerType"},
     {10, (const opcua_byte_t *)"ServerType"},
     NULL,
     0,
     NULL},
    {{0, MU_NODEID_NUMERIC, {62}},
     MU_NODECLASS_VARIABLETYPE,
     {16, (const opcua_byte_t *)"BaseVariableType"},
     {16, (const opcua_byte_t *)"BaseVariableType"},
     s_base_variable_type_refs,
     sizeof(s_base_variable_type_refs) / sizeof(s_base_variable_type_refs[0]),
     NULL},
    {{0, MU_NODEID_NUMERIC, {63}},
     MU_NODECLASS_VARIABLETYPE,
     {20, (const opcua_byte_t *)"BaseDataVariableType"},
     {20, (const opcua_byte_t *)"BaseDataVariableType"},
     s_base_data_variable_type_refs,
     sizeof(s_base_data_variable_type_refs) / sizeof(s_base_data_variable_type_refs[0]),
     NULL},
    {{0, MU_NODEID_NUMERIC, {68}},
     MU_NODECLASS_VARIABLETYPE,
     {12, (const opcua_byte_t *)"PropertyType"},
     {12, (const opcua_byte_t *)"PropertyType"},
     NULL,
     0,
     NULL}
#if MICRO_OPCUA_SUBSCRIPTIONS_STANDARD
    ,
    {{0, MU_NODEID_NUMERIC, {11492}},
     MU_NODECLASS_METHOD,
     {17, (const opcua_byte_t *)"GetMonitoredItems"},
     {17, (const opcua_byte_t *)"GetMonitoredItems"},
     s_get_monitored_items_refs,
     sizeof(s_get_monitored_items_refs) / sizeof(s_get_monitored_items_refs[0]),
     NULL},
    {{0, MU_NODEID_NUMERIC, {11493}},
     MU_NODECLASS_VARIABLE,
     {14, (const opcua_byte_t *)"InputArguments"},
     {14, (const opcua_byte_t *)"InputArguments"},
     s_property_type_ref,
     sizeof(s_property_type_ref) / sizeof(s_property_type_ref[0]),
     NULL},
    {{0, MU_NODEID_NUMERIC, {11494}},
     MU_NODECLASS_VARIABLE,
     {15, (const opcua_byte_t *)"OutputArguments"},
     {15, (const opcua_byte_t *)"OutputArguments"},
     s_property_type_ref,
     sizeof(s_property_type_ref) / sizeof(s_property_type_ref[0]),
     NULL},
    {{0, MU_NODEID_NUMERIC, {12873}},
     MU_NODECLASS_METHOD,
     {10, (const opcua_byte_t *)"ResendData"},
     {10, (const opcua_byte_t *)"ResendData"},
     s_resend_data_refs,
     sizeof(s_resend_data_refs) / sizeof(s_resend_data_refs[0]),
     NULL},
    {{0, MU_NODEID_NUMERIC, {12874}},
     MU_NODECLASS_VARIABLE,
     {14, (const opcua_byte_t *)"InputArguments"},
     {14, (const opcua_byte_t *)"InputArguments"},
     s_property_type_ref,
     sizeof(s_property_type_ref) / sizeof(s_property_type_ref[0]),
     NULL}
#endif
#endif
};

static const mu_address_space_t s_base_space = {s_base_nodes, sizeof(s_base_nodes) / sizeof(s_base_nodes[0])};
#else
static const mu_address_space_t s_base_space = {NULL, 0};
#endif /* MICRO_OPCUA_BASE_NODES */

const mu_address_space_t *mu_base_address_space(void) {
    return &s_base_space;
}

#ifdef MICRO_OPCUA_BASE_NODES
static opcua_statuscode_t base_status_time_read(void *ctx, const mu_nodeid_t *id, mu_variant_t *v) {
    const mu_base_runtime_t *rt = (const mu_base_runtime_t *)ctx;

    *v = (mu_variant_t){0};
    v->type = MU_TYPE_DATETIME;
    if (id->identifier.numeric == 2257u) {
        v->value.dt = rt->start_time;
    } else {
        v->value.dt = (rt->time && rt->time->get_time) ? rt->time->get_time(rt->time->context) : 0;
    }
    return MU_STATUS_GOOD;
}
#endif /* MICRO_OPCUA_BASE_NODES */

void mu_base_runtime_init(mu_base_runtime_nodes_t *s, const mu_time_adapter_t *time, opcua_datetime_t start_time) {
    s->rt.time = time;
    s->rt.start_time = start_time;

#ifdef MICRO_OPCUA_BASE_NODES
    s->values[0].type = MU_VALUESOURCE_CALLBACK;
    s->values[0].data.callback.read = base_status_time_read;
    s->values[0].data.callback.context = &s->rt;
    s->values[1].type = MU_VALUESOURCE_CALLBACK;
    s->values[1].data.callback.read = base_status_time_read;
    s->values[1].data.callback.context = &s->rt;

    s->nodes[0] = (mu_node_t){
        .node_id = {.namespace_index = 0, .identifier_type = MU_NODEID_NUMERIC, .identifier.numeric = 2258u},
        .node_class = MU_NODECLASS_VARIABLE,
        .browse_name = {.length = (opcua_int32_t)11, .data = (const opcua_byte_t *)"CurrentTime"},
        .display_name = {.length = (opcua_int32_t)11, .data = (const opcua_byte_t *)"CurrentTime"},
#if MICRO_OPCUA_BASE_TYPE_SYSTEM
        .references = s_property_type_ref,
        .reference_count = sizeof(s_property_type_ref) / sizeof(s_property_type_ref[0]),
#else
        .references = NULL,
        .reference_count = 0,
#endif
        .value = &s->values[0]};
    s->nodes[1] = (mu_node_t){
        .node_id = {.namespace_index = 0, .identifier_type = MU_NODEID_NUMERIC, .identifier.numeric = 2257u},
        .node_class = MU_NODECLASS_VARIABLE,
        .browse_name = {.length = (opcua_int32_t)9, .data = (const opcua_byte_t *)"StartTime"},
        .display_name = {.length = (opcua_int32_t)9, .data = (const opcua_byte_t *)"StartTime"},
#if MICRO_OPCUA_BASE_TYPE_SYSTEM
        .references = s_property_type_ref,
        .reference_count = sizeof(s_property_type_ref) / sizeof(s_property_type_ref[0]),
#else
        .references = NULL,
        .reference_count = 0,
#endif
        .value = &s->values[1]};

    s->space.nodes = s->nodes;
    s->space.node_count = 2;
#else
    s->space.nodes = NULL;
    s->space.node_count = 0;
#endif /* MICRO_OPCUA_BASE_NODES */
}

const mu_node_t *mu_resolve_node(const mu_address_space_t *user, mu_address_space_index_t *user_index,
                                 const mu_address_space_t *dynamic, const mu_nodeid_t *node_id) {
    if (user) {
        const mu_node_t *n = mu_address_space_find_node(user, user_index, node_id);
        if (n)
            return n;
    }
    if (dynamic) {
        const mu_node_t *n = mu_address_space_find_node(dynamic, NULL, node_id);
        if (n)
            return n;
    }
    return mu_address_space_find_node(mu_base_address_space(), NULL, node_id);
}
