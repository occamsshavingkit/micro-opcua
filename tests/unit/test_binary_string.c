/* tests/unit/test_binary_string.c */
#include "unity.h"
#include "micro_opcua/micro_opcua.h"

void setUp(void) {}
void tearDown(void) {}

/* These tests assume binary encoding APIs that will be implemented in subsequent tasks */
void test_binary_string_roundtrip(void) {
    opcua_byte_t buffer[128];
    mu_binary_writer_t writer;
    mu_binary_reader_t reader;
    
    mu_string_t empty_str = { 0, NULL };
    mu_string_t null_str = { -1, NULL };
    mu_string_t valid_str = { 4, (const opcua_byte_t*)"test" };
    
    mu_binary_writer_init(&writer, buffer, sizeof(buffer));
    TEST_ASSERT_EQUAL(MU_STATUS_GOOD, mu_binary_write_string(&writer, &empty_str));
    TEST_ASSERT_EQUAL(MU_STATUS_GOOD, mu_binary_write_string(&writer, &null_str));
    TEST_ASSERT_EQUAL(MU_STATUS_GOOD, mu_binary_write_string(&writer, &valid_str));
    
    mu_binary_reader_init(&reader, buffer, writer.position);
    mu_string_t read_str;
    
    TEST_ASSERT_EQUAL(MU_STATUS_GOOD, mu_binary_read_string(&reader, &read_str));
    TEST_ASSERT_EQUAL(0, read_str.length);
    
    TEST_ASSERT_EQUAL(MU_STATUS_GOOD, mu_binary_read_string(&reader, &read_str));
    TEST_ASSERT_EQUAL(-1, read_str.length);
    
    TEST_ASSERT_EQUAL(MU_STATUS_GOOD, mu_binary_read_string(&reader, &read_str));
    TEST_ASSERT_EQUAL(4, read_str.length);
    TEST_ASSERT_EQUAL_MEMORY("test", read_str.data, 4);
}

void test_binary_string_over_limit(void) {
    opcua_byte_t buffer[128];
    mu_binary_writer_t writer;
    mu_binary_reader_t reader;
    
    /* 65-byte string exceeds our 64-byte limit for strings */
    opcua_byte_t long_str_data[65] = {0};
    mu_string_t long_str = { 65, long_str_data };
    
    mu_binary_writer_init(&writer, buffer, sizeof(buffer));
    TEST_ASSERT_EQUAL(MU_STATUS_BAD_ENCODINGLIMITSEXCEEDED, mu_binary_write_string(&writer, &long_str));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_binary_string_roundtrip);
    RUN_TEST(test_binary_string_over_limit);
    return UNITY_END();
}
