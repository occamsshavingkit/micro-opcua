/* tests/unit/test_platform_adapter_contract.c */

#include "micro_opcua/platform.h"
#include <unity.h>

void setUp(void) {}
void tearDown(void) {}

void test_platform_adapter_null_callbacks(void)
{
    /* TBD: implement test */
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_platform_adapter_null_callbacks);
    return UNITY_END();
}
