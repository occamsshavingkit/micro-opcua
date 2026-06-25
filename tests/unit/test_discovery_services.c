/* tests/unit/test_discovery_services.c */
#include "unity.h"
#include "micro_opcua/micro_opcua.h"

void setUp(void) {}
void tearDown(void) {}

void test_discovery_findservers_response(void) {
    TEST_IGNORE_MESSAGE("Implement FindServers response fields test");
}

void test_discovery_getendpoints_response(void) {
    TEST_IGNORE_MESSAGE("Implement GetEndpoints response fields test");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_discovery_findservers_response);
    RUN_TEST(test_discovery_getendpoints_response);
    return UNITY_END();
}
