/* tests/unit/test_binary_string_errors.c */
#include "micro_opcua/micro_opcua.h"
#include "unity.h"
#include <stdint.h>

void setUp(void) {}
void tearDown(void) {}

void test_binary_string_truncated(void) {
    opcua_byte_t buffer[6] = {4, 0, 0, 0, 'A', 'B'}; /* Length 4, but only 2 bytes of data */
    mu_binary_reader_t reader;
    mu_binary_reader_init(&reader, buffer, sizeof(buffer));

    mu_string_t str;
    TEST_ASSERT_EQUAL(MU_STATUS_BAD_DECODINGERROR, mu_binary_read_string(&reader, &str));
}

void test_binary_string_negative_length(void) {
    opcua_byte_t buffer[4] = {0xFE, 0xFF, 0xFF, 0xFF}; /* Length -2 */
    mu_binary_reader_t reader;
    mu_binary_reader_init(&reader, buffer, sizeof(buffer));

    mu_string_t str;
    TEST_ASSERT_EQUAL(MU_STATUS_BAD_DECODINGERROR, mu_binary_read_string(&reader, &str));
}

void test_binary_string_excessive_length(void) {
    opcua_byte_t buffer[4] = {0x01, 0x10, 0x00, 0x00}; /* Length 4097, > MU_MAX_ENCODED_STRING_LENGTH (4096) */
    mu_binary_reader_t reader;
    mu_binary_reader_init(&reader, buffer, sizeof(buffer));

    mu_string_t str;
    TEST_ASSERT_EQUAL(MU_STATUS_BAD_ENCODINGLIMITSEXCEEDED, mu_binary_read_string(&reader, &str));
}

void test_binary_string_overdeclared_length_returns_bad_decoding_error(void) {
    /* OPC-10000-6 section 5.2: declared String length must fit in the remaining encoded bytes. */
    opcua_byte_t buffer[7] = {5, 0, 0, 0, 'O', 'P', 'C'};
    mu_binary_reader_t reader;
    mu_binary_reader_init(&reader, buffer, sizeof(buffer));

    mu_string_t str;
    TEST_ASSERT_EQUAL(MU_STATUS_BAD_DECODINGERROR, mu_binary_read_string(&reader, &str));
}

void test_binary_bytestring_overdeclared_length_returns_bad_decoding_error(void) {
    /* OPC-10000-6 section 5.2: declared ByteString length must fit in the remaining encoded bytes. */
    opcua_byte_t buffer[7] = {5, 0, 0, 0, 0xDE, 0xAD, 0xBE};
    mu_binary_reader_t reader;
    mu_binary_reader_init(&reader, buffer, sizeof(buffer));

    mu_bytestring_t bytes;
    TEST_ASSERT_EQUAL(MU_STATUS_BAD_DECODINGERROR, mu_binary_read_bytestring(&reader, &bytes));
}

void test_binary_string_reader_position_overflow_returns_bad_decoding_error(void) {
    /* OPC-10000-6 section 5.2: declared String length plus reader position must not overflow. */
    opcua_byte_t backing[5] = {1, 0, 0, 0, 'A'};
    mu_binary_reader_t reader;
    size_t overflow_position = SIZE_MAX - 4u;

    mu_binary_reader_init(&reader, (const opcua_byte_t *)((uintptr_t)&backing[0] - (uintptr_t)overflow_position),
                          SIZE_MAX);
    reader.position = overflow_position;

    mu_string_t str;
    TEST_ASSERT_EQUAL(MU_STATUS_BAD_DECODINGERROR, mu_binary_read_string(&reader, &str));
}

void test_binary_string_embedded_overrun(void) {
    /* If the length fits in MU_MAX_STRING_LENGTH, but is larger than the rest of the stream */
    opcua_byte_t buffer[6] = {4, 0, 0, 0, 'A', 'B'};
    mu_binary_reader_t reader;
    mu_binary_reader_init(&reader, buffer, sizeof(buffer));

    mu_string_t str;
    TEST_ASSERT_EQUAL(MU_STATUS_BAD_DECODINGERROR, mu_binary_read_string(&reader, &str));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_binary_string_truncated);
    RUN_TEST(test_binary_string_negative_length);
    RUN_TEST(test_binary_string_excessive_length);
    RUN_TEST(test_binary_string_overdeclared_length_returns_bad_decoding_error);
    RUN_TEST(test_binary_bytestring_overdeclared_length_returns_bad_decoding_error);
    RUN_TEST(test_binary_string_reader_position_overflow_returns_bad_decoding_error);
    RUN_TEST(test_binary_string_embedded_overrun);
    return UNITY_END();
}
