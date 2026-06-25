/* tests/unit/test_binary_nodeid_errors.c */
#include "unity.h"
#include "micro_opcua/micro_opcua.h"

void setUp(void) {}
void tearDown(void) {}

void test_binary_nodeid_invalid_encoding_mask(void) {
    opcua_byte_t buffer[2] = {0x06, 0x00}; /* 0x06 is invalid encoding format */
    mu_binary_reader_t reader;
    mu_binary_reader_init(&reader, buffer, sizeof(buffer));
    
    mu_nodeid_t node_id;
    TEST_ASSERT_EQUAL(MU_STATUS_BAD_DECODINGERROR, mu_binary_read_nodeid(&reader, &node_id));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_binary_nodeid_invalid_encoding_mask);
    return UNITY_END();
}
