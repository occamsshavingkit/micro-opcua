/* tests/unit/test_public_headers.c */

/*
 * This test simply includes all public headers to ensure they are
 * self-contained and compile cleanly with warnings as errors.
 */

#include "micro_opcua/config.h"
#include "micro_opcua/micro_opcua.h"
#include "micro_opcua/platform.h"
#include "micro_opcua/server.h"
#include "micro_opcua/status.h"
#include "micro_opcua/types.h"

#include <unity.h>

void setUp(void) {}
void tearDown(void) {}

void test_public_headers_compile(void) {
    TEST_ASSERT_TRUE(true);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_public_headers_compile);
    return UNITY_END();
}
