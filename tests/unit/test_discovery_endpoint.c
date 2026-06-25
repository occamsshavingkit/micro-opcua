/* tests/unit/test_discovery_endpoint.c */
#include "unity.h"
#include "micro_opcua/micro_opcua.h"

void setUp(void) {}
void tearDown(void) {}

#include "../../src/core/service_dispatch.h"

void test_findservers_no_session(void) {
    mu_server_t server;
    opcua_byte_t req[10];
    opcua_byte_t resp[10];
    size_t resp_len = 10;
    
    opcua_statuscode_t status = mu_service_dispatch(&server, MU_ID_FINDSERVERSREQUEST, req, 10, resp, &resp_len);
    TEST_ASSERT_EQUAL(MU_STATUS_GOOD, status);
}

void test_getendpoints_no_session(void) {
    mu_server_t server;
    opcua_byte_t req[10];
    opcua_byte_t resp[10];
    size_t resp_len = 10;
    
    opcua_statuscode_t status = mu_service_dispatch(&server, MU_ID_GETENDPOINTSREQUEST, req, 10, resp, &resp_len);
    TEST_ASSERT_EQUAL(MU_STATUS_GOOD, status);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_findservers_no_session);
    RUN_TEST(test_getendpoints_no_session);
    return UNITY_END();
}
