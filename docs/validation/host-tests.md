# Host Tests Validation

- Total elapsed time: < 15 minutes.
- Tests executed: 40 tests passed out of 40.
- Target SC-001 met: Yes.

## Feature 005 US1

- Default host suite: `ctest --test-dir build/test --output-on-failure`
  - Result: 58 tests passed out of 58.
- Standard DataChange host suite:
  `ctest --test-dir build/us1-standard --output-on-failure`
  - Build flags: `MICRO_OPCUA_SUBSCRIPTIONS_STANDARD=ON`,
    `MU_MAX_MONITORED_ITEMS=100`, `MU_MAX_SUBSCRIPTIONS=2`,
    `MU_MAX_PUBLISH_REQUESTS=5`, `MU_MONITORED_QUEUE_DEPTH=2`.
  - Result: 58 tests passed out of 58.

## Feature 005 US2

- Default type-system regression:
  `cmake --build build/test --target test_type_system && ctest --test-dir build/test --output-on-failure -R test_type_system`
  - Result: 1 test passed out of 1.
- Base Type System focused suite:
  `cmake --build build/us2-types --target test_type_system && ctest --test-dir build/us2-types --output-on-failure -R test_type_system`
  - Build flag: `MICRO_OPCUA_BASE_TYPE_SYSTEM=ON`.
  - Result: 1 test passed out of 1.
- Default host suite: `ctest --test-dir build/test --output-on-failure`
  - Result: 59 tests passed out of 59.
- Base Type System host suite:
  `ctest --test-dir build/us2-types --output-on-failure`
  - Build flag: `MICRO_OPCUA_BASE_TYPE_SYSTEM=ON`.
  - Result: 59 tests passed out of 59.

## Feature 005 US3

- Default Call gated-placeholder tests:
  `ctest --test-dir build/test --output-on-failure -R 'test_method_call'`
  - Result: 2 tests passed out of 2.
- Embedded-capacity Call focused suite:
  `ctest --test-dir build/us3-call --output-on-failure -R 'test_method_call'`
  - Build flags: `MICRO_OPCUA_SUBSCRIPTIONS_STANDARD=ON`,
    `MICRO_OPCUA_BASE_TYPE_SYSTEM=ON`, `MU_MAX_MONITORED_ITEMS=100`,
    `MU_MAX_SUBSCRIPTIONS=2`, `MU_MAX_PUBLISH_REQUESTS=5`,
    `MU_MONITORED_QUEUE_DEPTH=2`, `MU_MAX_TRIGGER_LINKS=4`.
  - Result: 2 tests passed out of 2.
- Default host suite: `ctest --test-dir build/test --output-on-failure`
  - Result: 61 tests passed out of 61.
- Embedded-capacity host suite:
  `ctest --test-dir build/us3-call --output-on-failure`
  - Result: 61 tests passed out of 61.

## Feature 005 US4 / Polish

- Host unit + integration suite:
  `make test`
  - Result: 61 tests passed out of 61.
- Profile Makefile builds:
  `make nano`, `make micro`, `make embedded`
  - Result: all built `examples/minimal_server` successfully.
- Embedded profile Makefile flags:
  `make embedded`
  - Confirmed: `MICRO_OPCUA_EMBEDDED_PROFILE=ON`,
    `MU_MAX_MONITORED_ITEMS=100`, `MU_MAX_SUBSCRIPTIONS=2`,
    `MU_MAX_PUBLISH_REQUESTS=5`, `MU_MONITORED_QUEUE_DEPTH=2`,
    `MU_MAX_TRIGGER_LINKS=4`.
