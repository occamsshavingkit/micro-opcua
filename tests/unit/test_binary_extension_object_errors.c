/* tests/unit/test_binary_extension_object_errors.c */
#include "muc_opcua/muc_opcua.h"
#include "unity.h"

typedef opcua_statuscode_t (*test_extension_object_decoder_t)(mu_binary_reader_t *body_reader, void *context);

opcua_statuscode_t mu_binary_read_extension_object_body(mu_binary_reader_t *reader, size_t length,
                                                        test_extension_object_decoder_t decoder, void *context);

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
    opcua_byte_t buffer[6] = {0x00, 42,  0x01, 0xFE,
                              0xFF, 0xFF}; /* NodeId TwoByte(42), ByteString mask, length -2 (negative but not -1 is
                                              invalid, wait, our code rejects any negative) */
    mu_binary_reader_t reader;
    mu_binary_reader_init(&reader, buffer, sizeof(buffer));

    mu_nodeid_t type_id;
    size_t length;
    TEST_ASSERT_EQUAL(MU_STATUS_BAD_DECODINGERROR, mu_binary_read_extension_object_header(&reader, &type_id, &length));
}

void test_binary_extension_object_declared_body_overrun_returns_bad_decoding_error(void) {
    /* OPC-10000-6 section 5.2.2.15: ExtensionObject bodies carry an encoded-byte count. */
    opcua_byte_t buffer[8] = {0x00, 42, 0x01, 10, 0x00, 0x00, 0x00, 0x00};
    mu_binary_reader_t reader;
    mu_binary_reader_init(&reader, buffer, sizeof(buffer));

    TEST_ASSERT_EQUAL(MU_STATUS_BAD_DECODINGERROR, mu_binary_skip_extension_object(&reader));
}

static opcua_statuscode_t decode_one_byte_extension_object_body(mu_binary_reader_t *body_reader, void *context) {
    (void)context;

    opcua_byte_t value;
    return mu_binary_read_byte(body_reader, &value);
}

void test_binary_extension_object_declared_body_underrun_returns_bad_decoding_error(void) {
    /* OPC-10000-6 section 5.2.2.15: the body decoder must consume the declared encoded-byte count exactly. */
    opcua_byte_t buffer[11] = {0x00, 42, 0x01, 4, 0x00, 0x00, 0x00, 0xAA, 0xBB, 0xCC, 0xDD};
    mu_binary_reader_t reader;
    mu_binary_reader_init(&reader, buffer, sizeof(buffer));

    mu_nodeid_t type_id;
    size_t length;
    TEST_ASSERT_EQUAL(MU_STATUS_GOOD, mu_binary_read_extension_object_header(&reader, &type_id, &length));
    TEST_ASSERT_EQUAL_size_t(4, length);

    TEST_ASSERT_EQUAL(MU_STATUS_BAD_DECODINGERROR, mu_binary_read_extension_object_body(
                                                       &reader, length, decode_one_byte_extension_object_body, NULL));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_binary_extension_object_invalid_encoding);
    RUN_TEST(test_binary_extension_object_invalid_body_length);
    RUN_TEST(test_binary_extension_object_declared_body_overrun_returns_bad_decoding_error);
    RUN_TEST(test_binary_extension_object_declared_body_underrun_returns_bad_decoding_error);
    return UNITY_END();
}
