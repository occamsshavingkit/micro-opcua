/* tests/fuzz/fuzz_binary_types.c */
#include "micro_opcua/micro_opcua.h"
#include <stddef.h>
#include <stdint.h>

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    /* To be implemented in T148 */
    (void)data;
    (void)size;
    return 0;
}
