# Quickstart: Embedded Profile Completion

How to build, run, and exercise the completed Embedded Device Server profile. Commands run
from the repo root. Claude runs all builds/tests/measurement; Codex only writes code.

## Build the embedded profile

```sh
make embedded          # Micro + Basic256Sha256 + Standard DataChange facet + type-system
```

This maps to the CMake options `-DMICRO_OPCUA_SECURITY=ON -DMICRO_OPCUA_SUBSCRIPTIONS=ON`
plus the Standard-facet capacity overrides (`-DMU_MAX_MONITORED_ITEMS=100 -DMU_MAX_SUBSCRIPTIONS=2
-DMU_MAX_PUBLISH_REQUESTS=5 -DMU_MONITORED_QUEUE_DEPTH=2`, research Decision 6). `make nano` and
`make micro` MUST remain unchanged in surface and footprint (small default capacities).

## Run the test suite

```sh
make test              # configures build/test with MICRO_OPCUA_BUILD_TESTS=ON and runs ctest
```

New test-first suites (added before implementation per the constitution):
- `test_subscriptions` extended: absolute deadband, queue≥2/discardOldest/overflow, SetTriggering.
- `test_method_call`: GetMonitoredItems, ResendData, and unimplemented-method rejection.
- type-system browse/read tests for the `Base Info Type System` node set + ServerProfileArray.
- malformed-input tests for the new decoders (DataChangeFilter deadband, SetTriggering, Call).

## Sanitizer + fuzz

```sh
cmake -S . -B build/asan -DMICRO_OPCUA_BUILD_TESTS=ON -DMICRO_OPCUA_SANITIZERS=address,undefined
cmake --build build/asan && ctest --test-dir build/asan
```

Fuzz targets extended to the new request/filter decoders.

## Measure size (size-discipline gate)

```sh
# Core .text for each profile, ARM Cortex-M0+ -Os (per docs/size/feature-size-ledger.md "How")
scripts/measure_size.sh all
```

Record the new `embedded` flash/RAM figures and the `MU_SERVER_STORAGE_BYTES` delta in
`docs/size/feature-size-ledger.md`. Confirm `.bss = 0` and heap = 0 (`nm` shows no `.bss`
symbols in the core).

## Interop validation

The CI `interop` (asyncua) and `dotnet-interop` (.NET reference client) jobs exercise the live
server. For the Embedded facet, drive a client that: creates a subscription + monitored items
with an absolute-deadband filter and queueSize≥2, links items with SetTriggering, calls
GetMonitoredItems / ResendData, and browses the type system / reads ServerProfileArray.

## Acceptance mapping

| Story | Build/exercise | Pass condition |
|---|---|---|
| US1 | `make embedded` + extended `test_subscriptions` | deadband/queue/SetTriggering/capacity tests green, cited StatusCodes on errors, ASan-clean |
| US2 | type-system browse/read tests | mandated namespace-0 type nodes reachable, HasTypeDefinition resolves, ServerProfileArray advertises the profile |
| US3 | `test_method_call` | GetMonitoredItems returns handle pairs, ResendData re-reports on next publish, unknown method → cited StatusCode |
| US4 | `make all-profiles` + docs review | embedded advertises the profile surface; `docs/conformance/profile-embedded.md` enumerates CUs at `profile-targeting`; size ledger refreshed; nano/micro unchanged |
