/* tests/unit/test_dispatch_services.c
 * Dispatch-level tests: drive mu_service_dispatch with a crafted request body
 * (positioned after the request-type NodeId, as mu_server_poll delivers it) and
 * assert on the encoded response body (response-type NodeId + ResponseHeader +
 * service response). */
#include "unity.h"
#include "micro_opcua/micro_opcua.h"
#include "../../src/core/server_internal.h"
#include "../../src/core/service_dispatch.h"
#include "../../src/services/service_header.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

static opcua_datetime_t fake_time(void *c) { (void)c; return 0; }

/* Write a RequestHeader (OPC 10000-4 7.32) with the given requestHandle. */
static void write_request_header(mu_binary_writer_t *w, opcua_uint32_t handle) {
    mu_nodeid_t null_id = { 0, MU_NODEID_NUMERIC, { 0 } };
    mu_string_t null_str = { -1, NULL };
    mu_binary_write_nodeid(w, &null_id);                    /* authenticationToken */
    mu_binary_write_int64(w, 0);                            /* timestamp */
    mu_binary_write_uint32(w, handle);                      /* requestHandle */
    mu_binary_write_uint32(w, 0);                           /* returnDiagnostics */
    mu_binary_write_string(w, &null_str);                  /* auditEntryId */
    mu_binary_write_uint32(w, 0);                           /* timeoutHint */
    mu_binary_write_extension_object_header(w, &null_id, 0);/* additionalHeader */
}

/* Skip a ResponseHeader the reader is positioned at. */
static void skip_response_header(mu_binary_reader_t *r, opcua_uint32_t *handle, opcua_statuscode_t *result) {
    opcua_int64_t ts; opcua_byte_t diag, enc; opcua_int32_t strtbl; mu_nodeid_t addl;
    mu_binary_read_int64(r, &ts);
    mu_binary_read_uint32(r, handle);
    mu_binary_read_statuscode(r, result);
    mu_binary_read_byte(r, &diag);          /* serviceDiagnostics (empty) */
    mu_binary_read_int32(r, &strtbl);       /* stringTable (null) */
    mu_binary_read_nodeid(r, &addl);        /* additionalHeader typeId */
    mu_binary_read_byte(r, &enc);           /* additionalHeader encoding */
}

void test_dispatch_open_secure_channel(void) {
    mu_server_t server;
    memset(&server, 0, sizeof(server));
    mu_secure_channel_init(&server.secure_channel);
    server.config.time_adapter.get_time = fake_time;

    /* Build the OpenSecureChannelRequest body (after the request-type NodeId). */
    opcua_byte_t req[256];
    mu_binary_writer_t w;
    mu_binary_writer_init(&w, req, sizeof(req));
    write_request_header(&w, 42);
    mu_binary_write_uint32(&w, 0);          /* ClientProtocolVersion */
    mu_binary_write_uint32(&w, 0);          /* RequestType = ISSUE */
    mu_binary_write_uint32(&w, 1);          /* SecurityMode = NONE */
    mu_bytestring_t client_nonce = { -1, NULL };
    mu_binary_write_bytestring(&w, &client_nonce);
    mu_binary_write_uint32(&w, 3600000);    /* RequestedLifetime */
    size_t req_len = w.position;

    opcua_byte_t resp[256];
    size_t resp_len = sizeof(resp);
    TEST_ASSERT_EQUAL(MU_STATUS_GOOD,
        mu_service_dispatch(&server, MU_ID_OPENSECURECHANNELREQUEST, req, req_len, resp, &resp_len));
    TEST_ASSERT_TRUE(server.secure_channel.is_open);

    /* Parse the response body. */
    mu_binary_reader_t r;
    mu_binary_reader_init(&r, resp, resp_len);
    mu_nodeid_t type;
    TEST_ASSERT_EQUAL(MU_STATUS_GOOD, mu_binary_read_nodeid(&r, &type));
    TEST_ASSERT_EQUAL(MU_ID_OPENSECURECHANNELRESPONSE, type.identifier.numeric);

    opcua_uint32_t handle; opcua_statuscode_t result;
    skip_response_header(&r, &handle, &result);
    TEST_ASSERT_EQUAL(42, handle);
    TEST_ASSERT_EQUAL(MU_STATUS_GOOD, result);

    opcua_uint32_t server_proto, channel_id, token_id, revised;
    opcua_int64_t created;
    mu_binary_read_uint32(&r, &server_proto);
    mu_binary_read_uint32(&r, &channel_id);
    mu_binary_read_uint32(&r, &token_id);
    mu_binary_read_int64(&r, &created);
    mu_binary_read_uint32(&r, &revised);
    TEST_ASSERT_EQUAL(1, channel_id);
    TEST_ASSERT_EQUAL(1, token_id);
    TEST_ASSERT_EQUAL(3600000, revised);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_dispatch_open_secure_channel);
    return UNITY_END();
}
