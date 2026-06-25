/* tests/unit/test_read_service.c */
#include "unity.h"
#include "micro_opcua/micro_opcua.h"

void setUp(void) {}
void tearDown(void) {}

void test_read_service_scalar_values(void) {
    TEST_IGNORE_MESSAGE("Implement Read service test for scalar values");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_read_service_scalar_values);
    return UNITY_END();
}
