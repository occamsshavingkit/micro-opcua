/* tests/unit/test_service_dispatch.c */
#include "unity.h"
#include "micro_opcua/micro_opcua.h"

void setUp(void) {}
void tearDown(void) {}

#include "../../src/core/service_dispatch.h"

void test_service_dispatch_known_requests(void) {
    const mu_service_handler_t *handler;
    
    handler = mu_get_service_handler(MU_ID_FINDSERVERSREQUEST);
    TEST_ASSERT_NOT_NULL(handler);
    TEST_ASSERT_EQUAL(MU_ID_FINDSERVERSRESPONSE, handler->response_id);
    TEST_ASSERT_FALSE(handler->requires_session);

    handler = mu_get_service_handler(MU_ID_READREQUEST);
    TEST_ASSERT_NOT_NULL(handler);
    TEST_ASSERT_EQUAL(MU_ID_READRESPONSE, handler->response_id);
    TEST_ASSERT_TRUE(handler->requires_session);
}

void test_service_dispatch_unknown_request(void) {
    const mu_service_handler_t *handler = mu_get_service_handler(99999);
    TEST_ASSERT_NULL(handler);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_service_dispatch_known_requests);
    RUN_TEST(test_service_dispatch_unknown_request);
    return UNITY_END();
}
