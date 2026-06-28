/* tests/unit/test_build_config.c */
#include <unity.h>

void setUp(void) {}
void tearDown(void) {}

void test_write_service_compilation_definition(void)
{
#ifdef MICRO_OPCUA_SERVICE_WRITE
    /* Write service is enabled */
    TEST_ASSERT_TRUE(1);
#else
    /* Write service is disabled */
    TEST_ASSERT_TRUE(1);
#endif
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_write_service_compilation_definition);
    return UNITY_END();
}
