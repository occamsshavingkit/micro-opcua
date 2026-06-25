/* tests/unit/test_service_dispatch.c */
#include "unity.h"
#include "micro_opcua/micro_opcua.h"

void setUp(void) {}
void tearDown(void) {}

void test_service_dispatch_known_requests(void) {
    TEST_IGNORE_MESSAGE("Implement service dispatch tests for known requests");
}

void test_service_dispatch_unknown_request(void) {
    TEST_IGNORE_MESSAGE("Implement service dispatch unknown request test");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_service_dispatch_known_requests);
    RUN_TEST(test_service_dispatch_unknown_request);
    return UNITY_END();
}
