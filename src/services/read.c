/* src/services/read.c */
#include "read.h"
#include <stddef.h>

opcua_statuscode_t mu_read_request_decode(mu_binary_reader_t *reader, 
                                          mu_read_request_t *req,
                                          mu_read_value_id_t *nodes_array,
                                          size_t max_nodes)
{
    if (!reader || !req || !nodes_array) return MU_STATUS_BAD_INTERNALERROR;

    opcua_statuscode_t status;
    
    status = mu_binary_read_double(reader, &req->max_age);
    if (status != MU_STATUS_GOOD) return status;
    
    status = mu_binary_read_uint32(reader, &req->timestamps_to_return);
    if (status != MU_STATUS_GOOD) return status;
    
    opcua_int32_t no_of_nodes;
    status = mu_binary_read_int32(reader, &no_of_nodes);
    if (status != MU_STATUS_GOOD) return status;
    
    if (no_of_nodes < 0) {
        req->num_nodes_to_read = 0;
        return MU_STATUS_GOOD;
    }
    if ((size_t)no_of_nodes > max_nodes) {
        return MU_STATUS_BAD_TOOMANYOPERATIONS;
    }
    
    req->num_nodes_to_read = (size_t)no_of_nodes;
    req->nodes_to_read = nodes_array;
    
    for (size_t i = 0; i < req->num_nodes_to_read; ++i) {
        mu_read_value_id_t *node = &req->nodes_to_read[i];
        
        status = mu_binary_read_nodeid(reader, &node->node_id);
        if (status != MU_STATUS_GOOD) return status;
        
        status = mu_binary_read_uint32(reader, &node->attribute_id);
        if (status != MU_STATUS_GOOD) return status;
        
        status = mu_binary_read_string(reader, &node->index_range);
        if (status != MU_STATUS_GOOD) return status;
        
        status = mu_binary_read_uint16(reader, &node->data_encoding_namespace_index);
        if (status != MU_STATUS_GOOD) return status;
        
        status = mu_binary_read_string(reader, &node->data_encoding_name);
        if (status != MU_STATUS_GOOD) return status;
    }
    
    return MU_STATUS_GOOD;
}
