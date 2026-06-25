/* src/core/service_dispatch.h */
#ifndef MICRO_OPCUA_SERVICE_DISPATCH_H
#define MICRO_OPCUA_SERVICE_DISPATCH_H

#include "micro_opcua/server.h"
#include "micro_opcua/opcua_ids.h"
#include <stdbool.h>

typedef struct {
    opcua_uint32_t request_id;
    opcua_uint32_t response_id;
    bool requires_session;
} mu_service_handler_t;

const mu_service_handler_t* mu_get_service_handler(opcua_uint32_t request_id);

opcua_statuscode_t mu_service_dispatch(
    mu_server_t *server, 
    opcua_uint32_t request_id, 
    const opcua_byte_t *request_body, 
    size_t request_length, 
    opcua_byte_t *response_body, 
    size_t *response_length);

#endif /* MICRO_OPCUA_SERVICE_DISPATCH_H */
