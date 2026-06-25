/* tests/unit/test_binary_extension_object_errors.c */
#include "unity.h"
#include "micro_opcua/micro_opcua.h"

void setUp(void) {}
void tearDown(void) {}

void test_binary_extension_object_invalid_encoding(void) {
    opcua_byte_t buffer[4] = {0x00, 42, 0x03, 0x00}; /* NodeId TwoByte(42), invalid mask 0x03 */
    mu_binary_reader_t reader;
    mu_binary_reader_init(&reader, buffer, sizeof(buffer));
    
    mu_nodeid_t type_id;
    size_t length;
    TEST_ASSERT_EQUAL(MU_STATUS_BAD_DECODINGERROR, mu_binary_read_extension_object_header(&reader, &type_id, &length));
}

void test_binary_extension_object_invalid_body_length(void) {
    opcua_byte_t buffer[6] = {0x00, 42, 0x01, 0xFE, 0xFF, 0xFF}; /* NodeId TwoByte(42), ByteString mask, length -2 (negative but not -1 is invalid, wait, our code rejects any negative) */
    mu_binary_reader_t reader;
    mu_binary_reader_init(&reader, buffer, sizeof(buffer));
    
    mu_nodeid_t type_id;
    size_t length;
    TEST_ASSERT_EQUAL(MU_STATUS_BAD_DECODINGERROR, mu_binary_read_extension_object_header(&reader, &type_id, &length));
    
    /* Also test body length exceeding remaining buffer */
    opcua_byte_t buffer2[8] = {0x00, 42, 0x01, 10, 0x00, 0x00, 0x00, 0x00}; /* length 10, but buffer is 8 */
    mu_binary_reader_init(&reader, buffer2, sizeof(buffer2));
    TEST_ASSERT_EQUAL(MU_STATUS_BAD_DECODINGERROR, mu_binary_skip_extension_object(&reader));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_binary_extension_object_invalid_encoding);
    RUN_TEST(test_binary_extension_object_invalid_body_length);
    return UNITY_END();
}
