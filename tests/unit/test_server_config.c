/* tests/unit/test_server_config.c */

#include "micro_opcua/server.h"
#include "micro_opcua/status.h"
#include <unity.h>

void setUp(void) {}
void tearDown(void) {}

void test_server_config_null_buffers(void)
{
    /* TBD: implement test */
}

void test_server_config_invalid_endpoint(void)
{
    /* TBD: implement test */
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_server_config_null_buffers);
    RUN_TEST(test_server_config_invalid_endpoint);
    return UNITY_END();
}
