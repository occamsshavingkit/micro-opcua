/* platform/arduino/src/mu_arduino_adapter.h */
#ifndef MU_ARDUINO_ADAPTER_H
#define MU_ARDUINO_ADAPTER_H

#include "muc_opcua/platform.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize the Arduino adapters */
void mu_arduino_adapter_init(
    mu_tcp_adapter_t *tcp_adapter,
    mu_time_adapter_t *time_adapter,
    mu_entropy_adapter_t *entropy_adapter
);

#ifdef __cplusplus
}
#endif

#endif /* MU_ARDUINO_ADAPTER_H */
