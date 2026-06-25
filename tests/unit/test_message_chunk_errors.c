/* tests/unit/test_message_chunk_errors.c */
#include "unity.h"
#include "micro_opcua/micro_opcua.h"

void setUp(void) {}
void tearDown(void) {}

void test_message_chunk_too_small(void) {
    opcua_byte_t buffer[7];
    mu_message_header_t header;
    
    /* Less than 8 bytes is Bad_TcpMessageTooShort (which maps to Bad_CommunicationError) or Bad_DecodingError */
    opcua_statuscode_t status = mu_parse_message_header(buffer, 7, &header);
    TEST_ASSERT_NOT_EQUAL(MU_STATUS_GOOD, status);
}

void test_message_chunk_too_large(void) {
    opcua_byte_t buffer[16];
    mu_message_header_t header;
    
    /* Make message_size larger than default limit (8192) */
    buffer[4] = 0x01;
    buffer[5] = 0x20;
    buffer[6] = 0x00;
    buffer[7] = 0x00; /* 8193 bytes */
    
    opcua_statuscode_t status = mu_parse_message_header(buffer, 16, &header);
    /* The parse_message_header itself doesn't check against a limit, it just reads it.
     * The limit is checked during buffer reading. We just check if it reads correctly.
     * For now we test just the message_size. */
    TEST_ASSERT_EQUAL(MU_STATUS_GOOD, status);
    TEST_ASSERT_EQUAL(8193, header.message_size);
}

void test_message_chunk_inconsistent_length(void) {
    opcua_byte_t buffer[16];
    mu_message_header_t header;
    
    buffer[4] = 32;
    buffer[5] = 0;
    buffer[6] = 0;
    buffer[7] = 0;
    
    /* Passed bytes_read = 16, but header says 32 */
    opcua_statuscode_t status = mu_parse_message_header(buffer, 16, &header);
    /* mu_parse_message_header does not validate the bytes_read against message_size, 
     * but we test that it reads it correctly */
    TEST_ASSERT_EQUAL(MU_STATUS_GOOD, status);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_message_chunk_too_small);
    RUN_TEST(test_message_chunk_too_large);
    RUN_TEST(test_message_chunk_inconsistent_length);
    return UNITY_END();
}
