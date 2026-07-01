/* tests/unit/test_public_headers.c */

/*
 * This test simply includes all public headers to ensure they are
 * self-contained and compile cleanly with warnings as errors.
 */

#include "muc_opcua/config.h"
#include "muc_opcua/muc_opcua.h"
#include "muc_opcua/platform.h"
#include "muc_opcua/server.h"
#include "muc_opcua/status.h"
#include "muc_opcua/types.h"

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
