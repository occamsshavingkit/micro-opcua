# Traceability: Embedded Profile Completion (feature 005)

Maps the Embedded 2017 UA Server Profile conformance units to OPC-10000 sections and to the
implementation/test files that satisfy them. Authoritative membership: `specs/005-embedded-profile-completion/research.md`.

**Target profile**: Embedded 2017 UA Server Profile —
`http://opcfoundation.org/UA-Profile/Server/EmbeddedUA2017` (OPC-10000-7 §6.6.69) = Micro 2017
Server Profile + SecurityPolicy Basic256Sha256 + Standard DataChange Subscription 2017 Server
Facet (§6.6.17) + `Base Info Type System` CU. Status: `profile-targeting` (CTT pending).

## Conformance-unit → OPC section → file map

| Conformance unit (facet) | OPC-10000 citation | Impl file(s) | Test(s) | Status |
|---|---|---|---|---|
| `Monitored Items Deadband Filter` (Std DataChange) — absolute deadband | OPC-10000-4 §7.22.2 (DeadbandType=Absolute) | `src/services/subscription.c` (T013 eval), `src/core/service_dispatch.c` (T017 decode) | `test_subscriptions.c::test_monitored_item_absolute_deadband` | DONE |
| `Monitor MinQueueSize_02` (Std DataChange) — queue ≥2 + discardOldest + overflow | OPC-10000-4 §5.13.2, §7.20.1 (overflow) | `src/services/subscription.c` (T014), `src/core/service_dispatch.c` | `test_subscriptions.c::test_monitored_item_queue_overflow` | DONE |
| `Monitor Triggering` (Std DataChange) — SetTriggering | OPC-10000-4 §5.13.5, §5.13.1.6 | `src/services/subscription.c` (T015), `src/core/service_dispatch.c` (T018) | `test_subscriptions.c::test_set_triggering` | DONE |
| `Monitor Items 10` / `Monitor Items 100` (Std DataChange) | OPC-10000-4 §5.13.2 | `src/services/subscription.{c,h}`, `CMakeLists.txt`, `src/CMakeLists.txt` capacity `-D` plumbing | `tests/unit/test_subscriptions_capacity.c`, `tests/integration/test_subscriptions.c` | DONE |
| `Subscription Minimum 02` / `Subscription Publish Min 05` (Std DataChange) | OPC-10000-4 §5.14.2, §5.14.5 | `src/services/subscription.{c,h}`, `CMakeLists.txt`, `src/CMakeLists.txt` capacity `-D` plumbing | `tests/unit/test_subscriptions_capacity.c`, `tests/integration/test_subscriptions.c` | DONE |
| `Method Call` (Std DataChange 2017) — Call service | OPC-10000-4 §5.12.2.2 | `src/core/service_dispatch.c` | `tests/unit/test_method_call.c`, `tests/unit/test_method_call_errors.c`, `tests/fuzz/fuzz_call.c` | DONE |
| `Base Info GetMonitoredItems Method` (Std DataChange 2017) | OPC-10000-5 §8.3.2, §9.1; behavior via OPC-10000-4 §5.12.2.2 | `src/address_space/base_nodes.c`, `src/services/subscription.{c,h}`, `src/core/service_dispatch.c` | `tests/unit/test_method_call.c`, `tests/fixtures/embedded/call-get-monitored-items.hex` | DONE |
| `Base Info ResendData Method` (Std DataChange 2017) | OPC-10000-5 §8.3.2, §9.2; behavior via OPC-10000-4 §5.12.2.2 | `src/address_space/base_nodes.c`, `src/services/subscription.{c,h}`, `src/core/service_dispatch.c` | `tests/unit/test_method_call.c`, `tests/fixtures/embedded/call-resend-data.hex` | DONE |
| `Base Info Type System` (Embedded) — full ns-0 type exposure | OPC-10000-5 (standard NodeSet); OPC-10000-3 (model) | `src/address_space/base_nodes.c`, `include/micro_opcua/address_space.h`, `src/services/browse.c`, `CMakeLists.txt`, `src/CMakeLists.txt` | `tests/unit/test_type_system.c`, `tests/unit/test_view_services.c` | DONE |
| HasTypeDefinition instance→type | OPC-10000-3 §7.7 | `src/address_space/base_nodes.c`, `src/services/browse.c` | `tests/unit/test_type_system.c`, `tests/unit/test_view_services.c` | DONE |
| ServerProfileArray advertises profile | OPC-10000-5 (Server object) | `src/address_space/base_nodes.c` | `tests/unit/test_type_system.c` | DONE |
| `Security Default ApplicationInstance Certificate` (Embedded) | OPC-10000-7 §6.6.69 (already shipped) | `src/security/certificate.c` (existing) | existing cert tests | satisfied (verify in docs) |

## Unsupported / out-of-scope (cited rejection)

| Feature | Why out | Expected StatusCode |
|---|---|---|
| TransferSubscriptions (§5.14.7) | Client Redundancy Facet, not required | `Bad_ServiceUnsupported` (if called) |
| Percent / aggregate deadband (§7.22.2 / OPC-10000-8) | Data Access Server Facet | `Bad_MonitoredItemFilterUnsupported` |
| Arbitrary methods via Call | only GetMonitoredItems/ResendData implemented | `Bad_MethodInvalid` |

## US1 validation record

Recorded for the Standard DataChange Subscription 2017 facet delta:

| Check | Command | Result |
|---|---|---|
| Default host suite | `ctest --test-dir build/test --output-on-failure` | 58/58 passed |
| Standard host suite | `ctest --test-dir build/us1-standard --output-on-failure` with `MICRO_OPCUA_SUBSCRIPTIONS_STANDARD=ON`, `MU_MAX_MONITORED_ITEMS=100`, `MU_MAX_SUBSCRIPTIONS=2`, `MU_MAX_PUBLISH_REQUESTS=5`, `MU_MONITORED_QUEUE_DEPTH=2` | 58/58 passed |
| Standard ASan host suite | `ASAN_OPTIONS=detect_leaks=0 ctest --test-dir build/us1-standard-asan2 --output-on-failure` with explicit `-fsanitize=address` compiler/linker flags | 58/58 passed |
| Decoder fuzz target build | `cmake -S . -B build/fuzz -DMICRO_OPCUA_BUILD_FUZZERS=ON ... && cmake --build build/fuzz --target fuzz_subscription_filters` | blocked locally: CMake reports fuzzing requires Clang; current compiler is GCC 13.3 |

US1 host size proxy:

| Build | `libmicro_opcua.a` `.text` | `.data` | `.bss` | `sizeof(struct mu_server)` | `MU_SERVER_STORAGE_BYTES` |
|---|---:|---:|---:|---:|---:|
| default subscriptions/security host build | 93,145 B | 2,176 B | 0 B | 10,392 B | 10,496 B |
| Standard US1 capacity build | 101,955 B | 2,200 B | 0 B | 41,960 B | 45,696 B |
| delta | +8,810 B | +24 B | 0 B | +31,568 B | +35,200 B |

The Standard RAM growth is caller-provided storage from `MU_MAX_MONITORED_ITEMS=100`,
`MU_MONITORED_QUEUE_DEPTH=2`, trigger-link storage, and the parked Publish capacity. Heap use
remains zero by design.

## US2 validation record

Recorded for the Base Info Type System exposure slice:

| Check | Command | Result |
|---|---|---|
| Checklist gate | `awk ... specs/005-embedded-profile-completion/checklists/*.md` | requirements.md: 21/21 complete, PASS |
| Default type-system regression | `cmake --build build/test --target test_type_system && ctest --test-dir build/test --output-on-failure -R test_type_system` | 1/1 passed; default build keeps lower-profile type surface gated |
| Base Type System test | `cmake --build build/us2-types --target test_type_system && ctest --test-dir build/us2-types --output-on-failure -R test_type_system` with `MICRO_OPCUA_BASE_TYPE_SYSTEM=ON` | 1/1 passed |
| Default host suite | `ctest --test-dir build/test --output-on-failure` | 59/59 passed |
| Base Type System host suite | `ctest --test-dir build/us2-types --output-on-failure` with `MICRO_OPCUA_BASE_TYPE_SYSTEM=ON` | 59/59 passed |

US2 host size proxy:

| Build | `libmicro_opcua.a` `.text` | `.data` | `.bss` | `sizeof(struct mu_server)` | `MU_SERVER_STORAGE_BYTES` |
|---|---:|---:|---:|---:|---:|
| default subscriptions/security host build | 93,129 B | 2,176 B | 0 B | 10,392 B | 10,496 B |
| Base Type System build | 96,466 B | 5,520 B | 0 B | 10,392 B | 10,496 B |
| delta | +3,337 B | +3,344 B | 0 B | 0 B | 0 B |

The type-system slice adds gated address-space metadata and browse filtering support, with no
increase to caller-provided server storage and no library `.bss`. Heap use remains zero by
design.

## US3 validation record

Recorded for the Method Call slice (GetMonitoredItems and ResendData):

| Check | Command | Result |
|---|---|---|
| Red-state gated tests | `ctest --test-dir build/us3-call --output-on-failure -R 'test_method_call'` before implementation | failed as expected: method nodes absent and `MU_ID_CALLREQUEST` returned `Bad_ServiceUnsupported` |
| Default gated-placeholder tests | `ctest --test-dir build/test --output-on-failure -R 'test_method_call'` | 2/2 passed; default/nano/micro Call surface remains gated off |
| Embedded-capacity focused tests | `ctest --test-dir build/us3-call --output-on-failure -R 'test_method_call'` with Standard+BaseType gates and profile capacities | 2/2 passed |
| Default host suite | `ctest --test-dir build/test --output-on-failure` | 61/61 passed |
| Embedded-capacity host suite | `ctest --test-dir build/us3-call --output-on-failure` with Standard+BaseType gates and profile capacities | 61/61 passed |
| ASan focused suite | `ASAN_OPTIONS=detect_leaks=0 ctest --test-dir build/us3-call-asan --output-on-failure -R 'test_method_call'` | 2/2 passed |
| Call fuzz target | `cmake -S . -B build/us3-fuzz -DMICRO_OPCUA_BUILD_FUZZERS=ON ... -DCMAKE_C_COMPILER=clang && cmake --build build/us3-fuzz --target fuzz_call` | target built with Clang 18.1.3 |
| Fuzz smoke | `build/us3-fuzz/tests/fuzz/fuzz_call -runs=1 tests/fixtures/embedded/call-get-monitored-items.hex` | passed fixed-input smoke |

US3 host size proxy:

| Build | `libmicro_opcua.a` `.text` | `.data` | `.bss` | `sizeof(struct mu_server)` | `MU_SERVER_STORAGE_BYTES` |
|---|---:|---:|---:|---:|---:|
| default subscriptions/security host build | 93,129 B | 2,176 B | 0 B | 10,392 B | 10,496 B |
| Standard+BaseType+Call capacity build | 109,297 B | 6,008 B | 0 B | 41,960 B | 45,696 B |
| delta vs default | +16,168 B | +3,832 B | 0 B | +31,568 B | +35,200 B |

Compared with the US1 Standard-capacity storage build, US3 adds no caller-storage growth:
the `resend_data_pending` flag fits existing structure padding. Heap use remains zero by design.

## US4 size and profile-build record

ARM Cortex-M0+ Thumb `-Os` portable-core measurement, reproduced with
`scripts/measure_size.sh all`:

| Profile | Core `.text` | `.data` | `.bss` | Notes |
|---|---:|---:|---:|---|
| nano | 16,713 B | 0 B | 0 B | unchanged vs pre-feature lower-tier profile |
| micro | 22,919 B | 0 B | 0 B | unchanged vs pre-feature lower-tier profile |
| embedded | 35,628 B | 0 B | 0 B | full Embedded 2017 profile gates enabled |

Host profile build reference:

| Profile | `libmicro_opcua.a` `.text` | `minimal_server` ELF `.text` | Notes |
|---|---:|---:|---|
| nano | 32,169 B | 36,213 B | host runtime/adapter inflated |
| micro | 42,552 B | 47,072 B | host runtime/adapter inflated |
| embedded | 65,664 B | 79,264 B | host OpenSSL adapter and example storage included |
