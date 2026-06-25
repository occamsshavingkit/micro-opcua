/* tests/fuzz/fuzz_message_chunk.c */
#include <stdint.h>
#include <stddef.h>
#include "micro_opcua/micro_opcua.h"

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    /* To be implemented in T147 */
    (void)data;
    (void)size;
    return 0;
}
