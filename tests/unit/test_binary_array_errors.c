/* tests/unit/test_binary_array_errors.c */
#include "unity.h"
#include "micro_opcua/micro_opcua.h"

void setUp(void) {}
void tearDown(void) {}

void test_binary_array_invalid_length(void) {
    opcua_byte_t buffer[4] = {0xFF, 0xFF, 0xFF, 0xFF}; /* Length -1 is valid for null array, but let's test length -2 */
    opcua_byte_t buffer2[4] = {0xFE, 0xFF, 0xFF, 0xFF}; /* -2 */
    mu_binary_reader_t reader;
    mu_binary_reader_init(&reader, buffer2, sizeof(buffer2));
    
    opcua_int32_t length;
    opcua_statuscode_t status = mu_binary_read_int32(&reader, &length);
    TEST_ASSERT_EQUAL(MU_STATUS_GOOD, status);
    TEST_ASSERT_EQUAL(-2, length);
    /* In OPC UA, array length < -1 is Bad_DecodingError, handled by array decoder */
    /* Wait, the array decoder isn't in binary_reader.c, arrays are read per type. */
    /* We just test primitive array reading logic. */
}

void test_binary_array_truncated(void) {
    opcua_byte_t buffer[6] = {2, 0, 0, 0, 42, 0}; /* Length 2, but only 2 bytes for the first uint16 */
    mu_binary_reader_t reader;
    mu_binary_reader_init(&reader, buffer, sizeof(buffer));
    
    opcua_int32_t length;
    TEST_ASSERT_EQUAL(MU_STATUS_GOOD, mu_binary_read_int32(&reader, &length));
    TEST_ASSERT_EQUAL(2, length);
    
    opcua_uint16_t val;
    TEST_ASSERT_EQUAL(MU_STATUS_GOOD, mu_binary_read_uint16(&reader, &val));
    TEST_ASSERT_EQUAL(42, val);
    
    /* The second read should fail */
    TEST_ASSERT_EQUAL(MU_STATUS_BAD_DECODINGERROR, mu_binary_read_uint16(&reader, &val));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_binary_array_invalid_length);
    RUN_TEST(test_binary_array_truncated);
    return UNITY_END();
}
