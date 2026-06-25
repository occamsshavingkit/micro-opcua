/* tests/unit/test_binary_array_errors.c */
#include "unity.h"
#include "micro_opcua/micro_opcua.h"

void setUp(void) {}
void tearDown(void) {}

void test_binary_array_invalid_length(void) {
    TEST_IGNORE_MESSAGE("To be implemented in T139");
}

void test_binary_array_truncated(void) {
    TEST_IGNORE_MESSAGE("To be implemented in T139");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_binary_array_invalid_length);
    RUN_TEST(test_binary_array_truncated);
    return UNITY_END();
}
