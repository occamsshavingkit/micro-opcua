# Sanitizers Validation

This file contains historical ASan/UBSan evidence plus the active required-gate
status for `020-audit-hardening`. The legacy pass summary immediately below is
not a full-feature pass claim for `020-audit-hardening`; detailed sanitizer
evidence for this feature begins with the T086b record below and does not
establish a full-feature pass.

- Historical Address Sanitizer status: Passed for the Feature 005 runs recorded
  below.
- Historical Undefined Behavior Sanitizer status: Passed for the Feature 005
  runs recorded below.

## Feature 020 Audit Hardening Required Gate

- Required-gate status: PASS for ASan/UBSan after the 2026-06-30
  blocker-remediation refresh.
- Conformance status: profile-targeting only; no external CTT evidence is
  recorded here.
- Scope: FR-020 and SC-006 require release validation to fail closed until the
  required gates can detect regressions in malformed-input handling, exact
  StatusCodes, conformance documentation, memory-safety behavior,
  formatting/static-analysis, and embedded build readiness.
- Sanitizer gate: required for ASan/UBSan memory-safety coverage of malformed
  input, transport bounds, security/session misuse, filter/lifetime paths, and
  service-semantics regressions under the commands listed in
  `docs/validation/audit-hardening.md`.
- Companion required gates: conformance-document checks,
  formatting/static-analysis, and embedded readiness remain tracked in their own
  validation ledgers before release validation can succeed.
- Current outcome: the Feature 020 sanitizer gate passes after the HistoryRead
  continuation-point ownership fix and stale CreateSession fixture refresh. The
  historical Feature 005 results below remain historical context only.

### T086b Feature 020 ASan/UBSan Evidence: 2026-06-30

- Build directory: `build-san`.
- Configure command:
  `cmake -S . -B build-san -DMICRO_OPCUA_BUILD_TESTS=ON -DMICRO_OPCUA_SUBSCRIPTIONS_STANDARD=ON -DMICRO_OPCUA_BASE_TYPE_SYSTEM=ON -DMU_MAX_MONITORED_ITEMS=100 -DMU_MAX_SUBSCRIPTIONS=2 -DMU_MAX_PUBLISH_REQUESTS=5 -DMU_MONITORED_QUEUE_DEPTH=2 -DMU_MAX_TRIGGER_LINKS=4 -DCMAKE_C_FLAGS='-fsanitize=address,undefined -fno-omit-frame-pointer' -DCMAKE_EXE_LINKER_FLAGS='-fsanitize=address,undefined'`
  - Result: PASS, exit code `0`.
  - Compiler and flags from `build-san/CMakeCache.txt`: `CMAKE_C_COMPILER=/usr/bin/cc`;
    `CMAKE_C_FLAGS=-fsanitize=address,undefined -fno-omit-frame-pointer`;
    `CMAKE_EXE_LINKER_FLAGS=-fsanitize=address,undefined`.
  - Enabled knobs: `MICRO_OPCUA_BUILD_TESTS=ON`,
    `MICRO_OPCUA_SUBSCRIPTIONS_STANDARD=ON`,
    `MICRO_OPCUA_BASE_TYPE_SYSTEM=ON`, `MU_MAX_MONITORED_ITEMS=100`,
    `MU_MAX_SUBSCRIPTIONS=2`, `MU_MAX_PUBLISH_REQUESTS=5`,
    `MU_MONITORED_QUEUE_DEPTH=2`, `MU_MAX_TRIGGER_LINKS=4`.
  - Transcript: `build-san/validation-logs/020-audit-hardening-sanitizer-configure.log`.
- Build command: `cmake --build build-san`
  - Result: PASS, exit code `0`.
  - Produced `build-san/src/libmicro_opcua.a` and sanitizer-linked test
    executables under `build-san/tests/unit` and `build-san/tests/integration`.
  - Transcript: `build-san/validation-logs/020-audit-hardening-sanitizer-build.log`.
- Full sanitizer CTest command:
  `ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=print_stacktrace=1 ctest --test-dir build-san --output-on-failure`
  - Result: FAIL, exit code `8`; 87/95 CTest targets passed, 8 failed, total
    test time `4.43 sec`.
  - Failing targets: `test_conformance_docs`, `test_history`,
    `test_server_handshake`, `test_response_regression`, `test_view_services`,
    `test_user_auth_encrypted`, `test_user_auth_certificate`, and
    `test_event_notifications`.
  - Sanitizer blocker: `test_history` reports AddressSanitizer
    `stack-use-after-scope` while encoding a HistoryRead continuation point:
    `mu_binary_write_string` at `src/encoding/binary_string.c:67`,
    `mu_history_read_response_encode` at `src/services/history.c:166`,
    `handle_history_read` at `src/core/service_dispatch.c:3776`, reached by
    `test_history_read_pagination` at `tests/unit/test_history.c:630`.
  - The transcript did not report a UBSan `runtime error` line; the blocking
    sanitizer finding is ASan.
  - Transcript: `build-san/validation-logs/020-audit-hardening-sanitizer-ctest.log`.
- Focused malformed decoding and transport bounds command:
  `ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=print_stacktrace=1 ctest --test-dir build-san --output-on-failure -R 'test_(binary_array_errors|binary_extension_object_errors|binary_string_errors|message_chunk_errors|tcp_connection|dispatch_services)'`
  - Result: PASS, exit code `0`; 6/6 CTest targets passed, 0 failed, total
    test time `0.01 sec`.
  - Transcript: `build-san/validation-logs/020-audit-hardening-sanitizer-focused-malformed.log`.
- Focused security and session misuse command:
  `ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=print_stacktrace=1 ctest --test-dir build-san --output-on-failure -R 'test_(secure_channel|session|connection_multiplex|service_state_errors|security_identity_errors|user_auth_plaintext)'`
  - Result: PASS, exit code `0`; 7/7 CTest targets passed, 0 failed, total
    test time `0.02 sec`.
  - Transcript: `build-san/validation-logs/020-audit-hardening-sanitizer-focused-security-session.log`.
- Focused optional service lifetime and filter paths command:
  `ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=print_stacktrace=1 ctest --test-dir build-san --output-on-failure -R 'test_(subscriptions_errors|aggregate|query_service|history|node_management_errors|node_management)'`
  - Result: FAIL, exit code `8`; 5/6 CTest targets passed, 1 failed, total
    test time `0.07 sec`.
  - Failing target: `test_history`, with the same AddressSanitizer
    `stack-use-after-scope` in the HistoryRead continuation-point path listed
    above.
  - Transcript: `build-san/validation-logs/020-audit-hardening-sanitizer-focused-optional-services.log`.
- Historical T086b sanitizer-gate outcome: NOT PASS before remediation. This
  evidence supported focused ASan/UBSan PASS coverage only for malformed
  decoding/transport bounds and security/session misuse. The T102b refresh below
  supersedes this historical failure.
- Conformance claim status: profile-targeting only. This sanitizer evidence is
  memory-safety evidence and does not provide external conformance-lab evidence.

### T102b Feature 020 ASan/UBSan Remediation Refresh: 2026-06-30

The T086b failure above is historical pre-remediation evidence. The active
sanitizer blocker was fixed by giving HistoryRead continuation-point buffers a
lifetime that covers response encoding and by refreshing stale CreateSession
integration request bodies.

- Focused HistoryRead rerun:
  `ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=print_stacktrace=1 ctest --test-dir build-san -R '^test_history$' --output-on-failure`
  - Result: PASS, exit code `0`; 1/1 CTest target passed.
- Focused integration rerun:
  `ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=print_stacktrace=1 ctest --test-dir build-san -R '^(test_server_handshake|test_response_regression|test_view_services|test_user_auth_encrypted|test_user_auth_certificate|test_event_notifications)$' --output-on-failure`
  - Result: PASS, exit code `0`; 6/6 CTest targets passed.
- Full sanitizer CTest rerun:
  `ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=print_stacktrace=1 ctest --test-dir build-san --output-on-failure`
  - Result: PASS, exit code `0`; 95/95 CTest targets passed, total test time
    approximately `4.71 sec`; no ASan or UBSan finding was reported.

T102b sanitizer-gate outcome: PASS for the Feature 020 ASan/UBSan gate. This is
memory-safety evidence only and does not provide external OPC Foundation CTT
evidence.

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
