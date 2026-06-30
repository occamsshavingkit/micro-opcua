#include "micro_opcua/encoding.h"
#include "micro_opcua/pubsub.h"
#include "micro_opcua/status.h"
#include "unity.h"
#include <stdio.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_uadp_network_message_encoding(void) {
    static const mu_pubsub_field_t fields[] = {
        {.value = {MU_TYPE_UINT32, {.ui32 = 42u}}},
        {.value = {MU_TYPE_BOOLEAN, {.b = true}}},
    };
    mu_pubsub_connection_t conn = {.port = 4840, .enabled = true, .publisher_id = 0x12345678, .address = "239.0.0.1"};
    mu_pubsub_writer_group_t wg = {
        .writer_group_id = 1,
        .publishing_interval_ms = 1000,
        .dataset_writer = {.data_set_writer_id = 0x1002, .fields = fields, .field_count = 2}};

    opcua_byte_t buffer[64];
    size_t bytes_written = 0;

    opcua_statuscode_t status = mu_encode_uadp_network_message(&conn, &wg, buffer, sizeof(buffer), &bytes_written);
    TEST_ASSERT_EQUAL_HEX32(MU_STATUS_GOOD, status);

    /* OPC-10000-14 sections 7.2.4.4.2, 7.2.4.5.2, 7.2.4.5.4,
       and 7.2.4.5.5: UADP NetworkMessage with UInt32 PublisherId,
       one DataSetWriterId, one sized Data Key Frame, and Variant fields. */
    TEST_ASSERT_EQUAL_UINT32(19, bytes_written);
    TEST_ASSERT_EQUAL_HEX8(0xD1, buffer[0]);
    TEST_ASSERT_EQUAL_HEX8(0x02, buffer[1]);
    TEST_ASSERT_EQUAL_HEX8(0x78, buffer[2]);
    TEST_ASSERT_EQUAL_HEX8(0x56, buffer[3]);
    TEST_ASSERT_EQUAL_HEX8(0x34, buffer[4]);
    TEST_ASSERT_EQUAL_HEX8(0x12, buffer[5]);
    TEST_ASSERT_EQUAL_HEX8(0x01, buffer[6]); /* PayloadHeader Count */
    TEST_ASSERT_EQUAL_HEX8(0x02, buffer[7]);
    TEST_ASSERT_EQUAL_HEX8(0x10, buffer[8]); /* DataSetWriterId 0x1002 */
    TEST_ASSERT_EQUAL_HEX8(0x08, buffer[9]); /* DataSetMessage size */
    TEST_ASSERT_EQUAL_HEX8(0x00, buffer[10]);
    TEST_ASSERT_EQUAL_HEX8(0x01, buffer[11]); /* DataSetFlags1: valid, Variant field encoding */

    mu_binary_reader_t reader;
    mu_binary_reader_init(&reader, &buffer[12], bytes_written - 12);
    mu_variant_t out0;
    mu_variant_t out1;
    TEST_ASSERT_EQUAL_HEX32(MU_STATUS_GOOD, mu_binary_read_variant(&reader, &out0));
    TEST_ASSERT_EQUAL(MU_TYPE_UINT32, out0.type);
    TEST_ASSERT_EQUAL_UINT32(42u, out0.value.ui32);
    TEST_ASSERT_EQUAL_HEX32(MU_STATUS_GOOD, mu_binary_read_variant(&reader, &out1));
    TEST_ASSERT_EQUAL(MU_TYPE_BOOLEAN, out1.type);
    TEST_ASSERT_TRUE(out1.value.b);
    TEST_ASSERT_EQUAL_UINT32(bytes_written - 12, reader.position);
}

void test_uadp_network_message_rejects_too_many_fields(void) {
    mu_pubsub_field_t fields[MU_PUBSUB_MAX_FIELDS + 1];
    memset(fields, 0, sizeof(fields));
    for (size_t i = 0; i < sizeof(fields) / sizeof(fields[0]); ++i) {
        fields[i].value.type = MU_TYPE_UINT32;
        fields[i].value.value.ui32 = (opcua_uint32_t)i;
    }

    mu_pubsub_connection_t conn = {.port = 4840, .enabled = true, .publisher_id = 1u};
    mu_pubsub_writer_group_t wg = {
        .writer_group_id = 1,
        .publishing_interval_ms = 1000,
        .dataset_writer = {.data_set_writer_id = 1, .fields = fields, .field_count = MU_PUBSUB_MAX_FIELDS + 1}};

    opcua_byte_t buffer[128];
    size_t bytes_written = 123u;
    opcua_statuscode_t status = mu_encode_uadp_network_message(&conn, &wg, buffer, sizeof(buffer), &bytes_written);

    TEST_ASSERT_EQUAL_HEX32(MU_STATUS_BAD_TOOMANYOPERATIONS, status);
    TEST_ASSERT_EQUAL_UINT32(0, bytes_written);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_uadp_network_message_encoding);
    RUN_TEST(test_uadp_network_message_rejects_too_many_fields);
    return UNITY_END();
}
