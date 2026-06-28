# Sanitizers Validation

- Address Sanitizer: Passed
- Undefined Behavior Sanitizer: Passed

## Feature 005 US1

- Command: `ASAN_OPTIONS=detect_leaks=0 ctest --test-dir build/us1-standard-asan2 --output-on-failure`
- Build flags: `MICRO_OPCUA_SUBSCRIPTIONS_STANDARD=ON`,
  `MU_MAX_MONITORED_ITEMS=100`, `MU_MAX_SUBSCRIPTIONS=2`,
  `MU_MAX_PUBLISH_REQUESTS=5`, `MU_MONITORED_QUEUE_DEPTH=2`, explicit
  `-fsanitize=address` compiler/linker flags.
- Result: 58 tests passed out of 58.
- Note: the repository's `MICRO_OPCUA_SANITIZERS` cache variable is not currently wired to
  compiler/linker flags, so this run used explicit CMake `CMAKE_C_FLAGS` /
  `CMAKE_EXE_LINKER_FLAGS`.

## Feature 005 US3

- Command:
  `ASAN_OPTIONS=detect_leaks=0 ctest --test-dir build/us3-call-asan --output-on-failure -R 'test_method_call'`
- Build flags: `MICRO_OPCUA_SUBSCRIPTIONS_STANDARD=ON`,
  `MICRO_OPCUA_BASE_TYPE_SYSTEM=ON`, `MU_MAX_MONITORED_ITEMS=100`,
  `MU_MAX_SUBSCRIPTIONS=2`, `MU_MAX_PUBLISH_REQUESTS=5`,
  `MU_MONITORED_QUEUE_DEPTH=2`, `MU_MAX_TRIGGER_LINKS=4`, explicit
  `-fsanitize=address` compiler/linker flags.
- Result: 2 tests passed out of 2.

## Feature 005 US4 / Polish

- Command:
  `ASAN_OPTIONS=detect_leaks=0 ctest --test-dir build/us4-ubsan-asan --output-on-failure`
- Configure/build:
  `cmake -S . -B build/us4-ubsan-asan -DMICRO_OPCUA_BUILD_TESTS=ON -DMICRO_OPCUA_SUBSCRIPTIONS_STANDARD=ON -DMICRO_OPCUA_BASE_TYPE_SYSTEM=ON -DMU_MAX_MONITORED_ITEMS=100 -DMU_MAX_SUBSCRIPTIONS=2 -DMU_MAX_PUBLISH_REQUESTS=5 -DMU_MONITORED_QUEUE_DEPTH=2 -DMU_MAX_TRIGGER_LINKS=4 -DCMAKE_C_FLAGS='-fsanitize=address,undefined -fno-omit-frame-pointer' -DCMAKE_EXE_LINKER_FLAGS='-fsanitize=address,undefined'`
- Result: 61 tests passed out of 61.
