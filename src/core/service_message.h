/* src/core/service_message.h */
#ifndef MUC_OPCUA_SERVICE_MESSAGE_H
#define MUC_OPCUA_SERVICE_MESSAGE_H

#include "muc_opcua/opcua_types.h"
#include "muc_opcua/status.h"
#include "muc_opcua/types.h"
#include <stddef.h>

opcua_statuscode_t mu_parse_service_prefix(const opcua_byte_t *buffer, size_t length, size_t *offset,
                                           mu_nodeid_t *node_id);
opcua_statuscode_t mu_write_service_prefix(opcua_byte_t *buffer, size_t length, size_t *offset,
                                           const mu_nodeid_t *node_id);

#endif /* MUC_OPCUA_SERVICE_MESSAGE_H */
