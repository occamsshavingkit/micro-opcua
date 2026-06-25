/* tests/unit/test_read_service.c */
#include "unity.h"
#include "micro_opcua/micro_opcua.h"

void setUp(void) {}
void tearDown(void) {}

#include "../../src/services/read.h"
#include "micro_opcua/encoding.h"

void test_read_service_request_decode(void) {
    opcua_byte_t buffer[256];
    mu_binary_writer_t writer;
    mu_binary_writer_init(&writer, buffer, sizeof(buffer));
    
    mu_binary_write_double(&writer, 0.0); /* MaxAge */
    mu_binary_write_uint32(&writer, MU_TIMESTAMPS_TO_RETURN_BOTH);
    mu_binary_write_int32(&writer, 1); /* Array length */
    
    mu_nodeid_t test_node = {.identifier_type = MU_NODEID_NUMERIC, .namespace_index = 1, .identifier.numeric = 100};
    mu_binary_write_nodeid(&writer, &test_node);
    mu_binary_write_uint32(&writer, MU_ATTRIBUTEID_VALUE);
    
    mu_string_t empty_str = {.length = 0, .data = NULL};
    mu_binary_write_string(&writer, &empty_str); /* IndexRange */
    mu_binary_write_uint16(&writer, 0); /* DataEncoding.NamespaceIndex */
    mu_binary_write_string(&writer, &empty_str); /* DataEncoding.Name */
    
    mu_binary_reader_t reader;
    mu_binary_reader_init(&reader, buffer, writer.position);
    
    mu_read_request_t req;
    mu_read_value_id_t nodes[2];
    TEST_ASSERT_EQUAL(MU_STATUS_GOOD, mu_read_request_decode(&reader, &req, nodes, 2));
    TEST_ASSERT_EQUAL(1, req.num_nodes_to_read);
    TEST_ASSERT_EQUAL(MU_TIMESTAMPS_TO_RETURN_BOTH, req.timestamps_to_return);
    TEST_ASSERT_EQUAL(100, req.nodes_to_read[0].node_id.identifier.numeric);
    TEST_ASSERT_EQUAL(MU_ATTRIBUTEID_VALUE, req.nodes_to_read[0].attribute_id);
}

void test_read_service_scalar_values(void) {
    TEST_IGNORE_MESSAGE("Implement Read service test for scalar values");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_read_service_request_decode);
    RUN_TEST(test_read_service_scalar_values);
    return UNITY_END();
}
