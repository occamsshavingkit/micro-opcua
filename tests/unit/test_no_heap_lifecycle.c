/* tests/unit/test_no_heap_lifecycle.c */

#include "micro_opcua/server.h"
#include <unity.h>
#include <stdlib.h>

void setUp(void) {}
void tearDown(void) {}

void test_lifecycle_no_allocations(void)
{
    /* TBD: implement test */
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_lifecycle_no_allocations);
    return UNITY_END();
}
