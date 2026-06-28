/* tests/fuzz/fuzz_message_chunk.c */
#include "micro_opcua/micro_opcua.h"
#include <stddef.h>
#include <stdint.h>

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    /* To be implemented in T147 */
    (void)data;
    (void)size;
    return 0;
}
