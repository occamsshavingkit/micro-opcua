/* src/services/browse.c */
#include "browse.h"
#include <stddef.h>

opcua_statuscode_t mu_browse_request_decode(mu_binary_reader_t *reader, 
                                            mu_browse_request_t *req,
                                            mu_browse_description_t *desc_array,
                                            size_t max_desc)
{
    if (!reader || !req || !desc_array) return MU_STATUS_BAD_INTERNALERROR;

    opcua_statuscode_t status;
    
    /* ViewDescription */
    status = mu_binary_read_nodeid(reader, &req->view_id);
    if (status != MU_STATUS_GOOD) return status;
    status = mu_binary_read_int64(reader, &req->timestamp);
    if (status != MU_STATUS_GOOD) return status;
    status = mu_binary_read_uint32(reader, &req->view_version);
    if (status != MU_STATUS_GOOD) return status;
    
    /* RequestedMaxReferencesPerNode */
    status = mu_binary_read_uint32(reader, &req->requested_max_references_per_node);
    if (status != MU_STATUS_GOOD) return status;
    
    /* NodesToBrowse Array Length */
    opcua_int32_t no_of_nodes;
    status = mu_binary_read_int32(reader, &no_of_nodes);
    if (status != MU_STATUS_GOOD) return status;
    
    if (no_of_nodes < 0) {
        req->num_nodes_to_browse = 0;
        return MU_STATUS_GOOD; /* null array */
    }
    if ((size_t)no_of_nodes > max_desc) {
        return MU_STATUS_BAD_INTERNALERROR; /* Too many items for static array */
    }
    
    req->num_nodes_to_browse = (size_t)no_of_nodes;
    req->nodes_to_browse = desc_array;
    
    for (size_t i = 0; i < req->num_nodes_to_browse; ++i) {
        mu_browse_description_t *desc = &req->nodes_to_browse[i];
        
        status = mu_binary_read_nodeid(reader, &desc->node_id);
        if (status != MU_STATUS_GOOD) return status;
        
        status = mu_binary_read_uint32(reader, &desc->browse_direction);
        if (status != MU_STATUS_GOOD) return status;
        
        status = mu_binary_read_nodeid(reader, &desc->reference_type_id);
        if (status != MU_STATUS_GOOD) return status;
        
        opcua_boolean_t include_subtypes;
        status = mu_binary_read_boolean(reader, &include_subtypes);
        desc->include_subtypes = include_subtypes;
        if (status != MU_STATUS_GOOD) return status;
        
        status = mu_binary_read_uint32(reader, &desc->node_class_mask);
        if (status != MU_STATUS_GOOD) return status;
        
        status = mu_binary_read_uint32(reader, &desc->result_mask);
        if (status != MU_STATUS_GOOD) return status;
    }
    
    return MU_STATUS_GOOD;
}
