/* tests/integration/test_discovery_endpoint_no_session.c */
#include "unity.h"
#include "micro_opcua/micro_opcua.h"

void setUp(void) {}
void tearDown(void) {}

void test_integration_findservers_and_getendpoints(void) {
    TEST_IGNORE_MESSAGE("Implement integration test for discovery without session");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_integration_findservers_and_getendpoints);
    return UNITY_END();
}
