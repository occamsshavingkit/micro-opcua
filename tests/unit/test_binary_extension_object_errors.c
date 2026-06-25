/* tests/unit/test_binary_extension_object_errors.c */
#include "unity.h"
#include "micro_opcua/micro_opcua.h"

void setUp(void) {}
void tearDown(void) {}

void test_binary_extension_object_invalid_encoding(void) {
    TEST_IGNORE_MESSAGE("To be implemented in T141");
}

void test_binary_extension_object_invalid_body_length(void) {
    TEST_IGNORE_MESSAGE("To be implemented in T141");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_binary_extension_object_invalid_encoding);
    RUN_TEST(test_binary_extension_object_invalid_body_length);
    return UNITY_END();
}
