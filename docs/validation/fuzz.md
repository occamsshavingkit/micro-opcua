# Fuzz Target Validation

- `fuzz_binary_reader`: Passed smoke test
- `fuzz_binary_types`: Passed smoke test
- `fuzz_message_chunk`: Passed smoke test

## Feature 005 US1

- Target added: `fuzz_subscription_filters` for DataChangeFilter and SetTriggering decoders.
- Local build attempt:
  `cmake -S . -B build/fuzz -DMICRO_OPCUA_BUILD_FUZZERS=ON -DMICRO_OPCUA_SUBSCRIPTIONS_STANDARD=ON -DMU_MAX_MONITORED_ITEMS=100 -DMU_MAX_SUBSCRIPTIONS=2 -DMU_MAX_PUBLISH_REQUESTS=5 -DMU_MONITORED_QUEUE_DEPTH=2 && cmake --build build/fuzz --target fuzz_subscription_filters`
- Result: blocked locally. CMake reports fuzzing requires Clang; current compiler is GCC 13.3,
  so the target is not generated in this environment.

## Feature 005 US3

- Target added: `fuzz_call` for the Call service decoder and the gated
  GetMonitoredItems/ResendData routing path.
- Local build:
  `cmake -S . -B build/us3-fuzz -DMICRO_OPCUA_BUILD_FUZZERS=ON -DMICRO_OPCUA_SUBSCRIPTIONS_STANDARD=ON -DMICRO_OPCUA_BASE_TYPE_SYSTEM=ON -DMU_MAX_MONITORED_ITEMS=100 -DMU_MAX_SUBSCRIPTIONS=2 -DMU_MAX_PUBLISH_REQUESTS=5 -DMU_MONITORED_QUEUE_DEPTH=2 -DMU_MAX_TRIGGER_LINKS=4 -DCMAKE_C_COMPILER=clang && cmake --build build/us3-fuzz --target fuzz_call`
- Result: built with Clang 18.1.3.
- Smoke:
  `build/us3-fuzz/tests/fuzz/fuzz_call -runs=1 tests/fixtures/embedded/call-get-monitored-items.hex`
  - Result: passed fixed-input smoke.

## Feature 005 US4 / Polish

- All fuzz targets built with Clang 18.1.3:
  `cmake --build build/us3-fuzz`
  - Targets: `fuzz_binary_reader`, `fuzz_message_chunk`, `fuzz_binary_types`,
    `fuzz_activate_session`, `fuzz_subscription_filters`, `fuzz_call`.
- Fixed-input smokes:
  `fuzz_call`, `fuzz_subscription_filters`, `fuzz_binary_reader`,
  `fuzz_binary_types`, `fuzz_message_chunk`, and `fuzz_activate_session`
  each ran with `-runs=1` against the embedded Call fixtures.
  - Result: all smokes passed. These are smoke executions, not long-running fuzz campaigns.
