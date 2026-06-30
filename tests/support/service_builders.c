/* tests/support/service_builders.c */
#include "service_builders.h"

void test_write_request_header(mu_binary_writer_t *writer, opcua_uint32_t auth_token, opcua_uint32_t request_handle) {
    mu_nodeid_t token = {0, MU_NODEID_NUMERIC, {auth_token}};
    mu_nodeid_t no_header = {0, MU_NODEID_NUMERIC, {0}};
    mu_string_t null_string = {-1, NULL};

    mu_binary_write_nodeid(writer, &token);
    mu_binary_write_int64(writer, 0);
    mu_binary_write_uint32(writer, request_handle);
    mu_binary_write_uint32(writer, 0);
    mu_binary_write_string(writer, &null_string);
    mu_binary_write_uint32(writer, 0);
    mu_binary_write_extension_object_header(writer, &no_header, 0);
}

void test_write_create_session_body(mu_binary_writer_t *writer, opcua_uint32_t request_handle,
                                    opcua_double_t requested_timeout) {
    mu_string_t null_string = {-1, NULL};
    mu_bytestring_t null_bytes = {-1, NULL};

    test_write_request_header(writer, 0, request_handle);
    mu_binary_write_string(writer, &null_string);    /* ClientDescription.applicationUri */
    mu_binary_write_string(writer, &null_string);    /* productUri */
    mu_binary_write_byte(writer, 0x00);              /* applicationName LocalizedText */
    mu_binary_write_uint32(writer, 1);               /* applicationType = CLIENT */
    mu_binary_write_string(writer, &null_string);    /* gatewayServerUri */
    mu_binary_write_string(writer, &null_string);    /* discoveryProfileUri */
    mu_binary_write_int32(writer, 0);                /* discoveryUrls[] */
    mu_binary_write_string(writer, &null_string);    /* serverUri */
    mu_binary_write_string(writer, &null_string);    /* endpointUrl */
    mu_binary_write_string(writer, &null_string);    /* sessionName */
    mu_binary_write_bytestring(writer, &null_bytes); /* clientNonce */
    mu_binary_write_bytestring(writer, &null_bytes); /* clientCertificate */
    mu_binary_write_double(writer, requested_timeout);
    mu_binary_write_uint32(writer, 0); /* maxResponseMessageSize */
}
