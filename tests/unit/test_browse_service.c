/* tests/unit/test_browse_service.c */
#include "unity.h"
#include "micro_opcua/micro_opcua.h"

void setUp(void) {}
void tearDown(void) {}

#include "../../src/services/browse.h"
#include "micro_opcua/binary_codec.h"

void test_browse_service_static_references(void) {
    opcua_byte_t buffer[256];
    mu_binary_writer_t writer;
    mu_binary_writer_init(&writer, buffer, sizeof(buffer));
    
    /* ViewDescription */
    mu_binary_encode_nodeid(&writer, &(mu_nodeid_t){.identifier_type = MU_NODEID_NUMERIC, .namespace_index = 0, .identifier.numeric = 0}); /* empty nodeid */
    mu_binary_encode_int64(&writer, 0); /* timestamp */
    mu_binary_encode_uint32(&writer, 0); /* view version */
    
    /* RequestedMaxReferencesPerNode */
    mu_binary_encode_uint32(&writer, 10);
    
    /* NoOfNodesToBrowse */
    mu_binary_encode_int32(&writer, 1);
    
    /* NodesToBrowse[0] */
    mu_binary_encode_nodeid(&writer, &(mu_nodeid_t){.identifier_type = MU_NODEID_NUMERIC, .namespace_index = 0, .identifier.numeric = 84}); /* RootFolder */
    mu_binary_encode_uint32(&writer, MU_BROWSE_DIRECTION_FORWARD);
    mu_binary_encode_nodeid(&writer, &(mu_nodeid_t){.identifier_type = MU_NODEID_NUMERIC, .namespace_index = 0, .identifier.numeric = 33}); /* HierarchicalReferences */
    mu_binary_encode_boolean(&writer, true);
    mu_binary_encode_uint32(&writer, 0xFFFFFFFF); /* All classes */
    mu_binary_encode_uint32(&writer, 0x3F); /* Result mask */
    
    mu_binary_reader_t reader;
    mu_binary_reader_init(&reader, buffer, writer.offset);
    
    mu_browse_request_t req;
    mu_browse_description_t desc[2];
    
    TEST_ASSERT_EQUAL(MU_STATUS_GOOD, mu_browse_request_decode(&reader, &req, desc, 2));
    TEST_ASSERT_EQUAL(10, req.requested_max_references_per_node);
    TEST_ASSERT_EQUAL(1, req.num_nodes_to_browse);
    TEST_ASSERT_EQUAL(MU_NODEID_NUMERIC, req.nodes_to_browse[0].node_id.identifier_type);
    TEST_ASSERT_EQUAL(84, req.nodes_to_browse[0].node_id.identifier.numeric);
    TEST_ASSERT_EQUAL(MU_BROWSE_DIRECTION_FORWARD, req.nodes_to_browse[0].browse_direction);
    TEST_ASSERT_TRUE(req.nodes_to_browse[0].include_subtypes);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_browse_service_static_references);
    return UNITY_END();
}
