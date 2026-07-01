/* tests/support/service_builders.h */
#ifndef TEST_SERVICE_BUILDERS_H
#define TEST_SERVICE_BUILDERS_H

#include "muc_opcua/muc_opcua.h"

#ifdef __cplusplus
extern "C" {
#endif

void test_write_request_header(mu_binary_writer_t *writer, opcua_uint32_t auth_token, opcua_uint32_t request_handle);
void test_write_create_session_body(mu_binary_writer_t *writer, opcua_uint32_t request_handle,
                                    opcua_double_t requested_timeout);

#ifdef __cplusplus
}
#endif

#endif /* TEST_SERVICE_BUILDERS_H */
