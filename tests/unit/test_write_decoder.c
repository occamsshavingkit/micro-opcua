/* tests/unit/test_write_decoder.c */
/* Normative References:
 * - OPC-10000-6 §5.2 (Binary encoding rules)
 * - OPC-10000-4 §5.11.4.2 (WriteValue parameters)
 */

#include "micro_opcua/encoding.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_decode_write_value_scalar_types(void) {
#ifdef MICRO_OPCUA_SERVICE_WRITE
    opcua_byte_t buffer[256];
    mu_binary_writer_t writer;
    mu_binary_writer_init(&writer, buffer, sizeof(buffer));

    /* NodeId: 4-byte numeric ns=1, identifier 5001 */
    mu_nodeid_t node_id = {1, MU_NODEID_NUMERIC, {.numeric = 5001}};
    mu_binary_write_nodeid(&writer, &node_id);

    /* AttributeId: 13 (Value) */
    mu_binary_write_int32(&writer, 13);

    /* IndexRange: null String (-1 length) */
    mu_string_t index_range = {-1, NULL};
    mu_binary_write_string(&writer, &index_range);

    /* DataValue: Int32 42 */
    mu_datavalue_t dv;
    dv.has_value = true;
    dv.has_status = false;
    dv.has_source_timestamp = false;
    dv.has_server_timestamp = false;
    dv.value.type = MU_TYPE_INT32;
    dv.value.is_array = false;
    dv.value.value.i32 = 42;
    mu_binary_write_datavalue(&writer, &dv);

    mu_binary_reader_t reader;
    mu_binary_reader_init(&reader, buffer, writer.position);

    mu_write_value_t wv;
    opcua_statuscode_t status = mu_write_value_decode(&reader, &wv);

    TEST_ASSERT_EQUAL(MU_STATUS_GOOD, status);
    TEST_ASSERT_EQUAL(1, wv.node_id.namespace_index);
    TEST_ASSERT_EQUAL(MU_NODEID_NUMERIC, wv.node_id.identifier_type);
    TEST_ASSERT_EQUAL(5001, wv.node_id.identifier.numeric);
    TEST_ASSERT_EQUAL(13, wv.attribute_id);
    TEST_ASSERT_EQUAL(-1, wv.index_range.length);
    TEST_ASSERT_TRUE(wv.value.has_value);
    TEST_ASSERT_EQUAL(MU_TYPE_INT32, wv.value.value.type);
    TEST_ASSERT_EQUAL(42, wv.value.value.value.i32);
#endif
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_decode_write_value_scalar_types);
    return UNITY_END();
}
