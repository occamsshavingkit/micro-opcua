/* src/core/sequence.h */
#ifndef MUC_OPCUA_SEQUENCE_H
#define MUC_OPCUA_SEQUENCE_H

#include "muc_opcua/opcua_types.h"
#include "muc_opcua/status.h"
#include <stdbool.h>

typedef struct {
    opcua_uint32_t last_sequence_number;
    bool is_initialized;
} mu_sequence_validator_t;

void mu_sequence_validator_init(mu_sequence_validator_t *validator);
opcua_statuscode_t mu_sequence_validate(mu_sequence_validator_t *validator, opcua_uint32_t sequence_number);

#endif /* MUC_OPCUA_SEQUENCE_H */
