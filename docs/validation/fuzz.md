# Fuzz Target Validation

Historical smoke summary from earlier validation:

- `fuzz_binary_reader`: Passed smoke test
- `fuzz_binary_types`: Passed smoke test
- `fuzz_message_chunk`: Passed smoke test

These top-level bullets are pre-Feature 020 historical notes only. They do not
claim that the `020-audit-hardening` fuzz gate passes.

## Feature 020 Audit Hardening

### T087a / US4: `fuzz_binary_reader`

Coverage and release-gate intent:

- Requirements: FR-001, FR-003, FR-020.
- Success criteria: SC-001, SC-006.
- Normative scope: OPC-10000-6 section 5.2 binary encoding length handling and
  OPC-10000-6 section 5.2.5 array length encoding.
- Target/source mapping: `tests/fuzz/CMakeLists.txt` defines
  `fuzz_binary_reader` from `tests/fuzz/fuzz_binary_reader.c`.
- Harness behavior checked: array counts below `-1` must remain
  `Bad_DecodingError`; overdeclared String and ByteString lengths must not read
  beyond the encoded input.
- Gate honesty: these rows record short libFuzzer smokes for the binary reader
  target only. Full Feature 020 fuzz-gate status also depends on the other
  target rows below, including the T102b `fuzz_binary_types` remediation refresh.

Commands and results:

| Step | Command | Result |
| --- | --- | --- |
| Configure | `cmake -S . -B build/fuzz -DMICRO_OPCUA_BUILD_FUZZERS=ON -DMICRO_OPCUA_SUBSCRIPTIONS_STANDARD=ON -DMICRO_OPCUA_BASE_TYPE_SYSTEM=ON -DMU_MAX_MONITORED_ITEMS=100 -DMU_MAX_SUBSCRIPTIONS=2 -DMU_MAX_PUBLISH_REQUESTS=5 -DMU_MONITORED_QUEUE_DEPTH=2 -DMU_MAX_TRIGGER_LINKS=4 -DCMAKE_C_COMPILER=clang` | PASS, exit code 0. CMake wrote build files to `build/fuzz`; Clang available as Ubuntu clang 18.1.3. Transcript: `/tmp/micro-opcua-t087a-fuzz-configure.log`. |
| Build target | `cmake --build build/fuzz --target fuzz_binary_reader` | PASS, exit code 0. Produced `build/fuzz/tests/fuzz/fuzz_binary_reader`. Transcript: `/tmp/micro-opcua-t087a-fuzz-build.log`. |
| Smoke | `build/fuzz/tests/fuzz/fuzz_binary_reader -runs=1` | PASS, exit code 0. libFuzzer completed `Done 2 runs in 0 second(s)` with no crash, timeout, OOM, or sanitizer finding reported. Transcript: `/tmp/micro-opcua-t087a-fuzz-binary-reader-smoke.log`. |
| T097a smoke refresh | `build/fuzz/tests/fuzz/fuzz_binary_reader -runs=1` | PASS, exit code 0. Used existing binary `build/fuzz/tests/fuzz/fuzz_binary_reader`; prerequisites were not rerun because the binary existed and was newer than relevant fuzz source/CMake files. No corpus or fixture was provided, so libFuzzer started from an empty corpus and completed `Done 2 runs in 0 second(s)` with no crash, timeout, OOM, or sanitizer finding reported. Transcript: `/tmp/micro-opcua-t097a/fuzz-binary-reader-smoke.stdout-stderr.log`; exit code file: `/tmp/micro-opcua-t097a/fuzz-binary-reader-smoke.exitcode`. |

### T087b / US4: `fuzz_message_chunk`

Coverage and release-gate intent:

- Requirements: FR-003, FR-010, FR-020.
- Success criteria: SC-001, SC-006.
- Normative scope: OPC-10000-6 section 6.7.2 MessageChunk structure and
  OPC-10000-6 section 7.1.2.2 Message Header.
- Target/source mapping: `tests/fuzz/CMakeLists.txt` defines
  `fuzz_message_chunk` from `tests/fuzz/fuzz_message_chunk.c`.
- Harness behavior checked: invalid TCP message types must remain
  `Bad_TcpMessageTypeInvalid`; declared message sizes must reject below-header
  and over-negotiated boundaries; abort chunks must return their encoded TCP
  Error without service-body dispatch; non-final continuation chunks must not
  dispatch as complete service requests in single-chunk mode.
- Gate honesty: this row records short libFuzzer smokes for the message chunk
  target only, including the fixed-input smoke named by
  `docs/validation/audit-hardening.md`. Full Feature 020 fuzz-gate status also
  depends on the other target rows below.

Commands and results:

| Step | Command | Result |
| --- | --- | --- |
| Configure | `cmake -S . -B build/fuzz -DMICRO_OPCUA_BUILD_FUZZERS=ON -DMICRO_OPCUA_SUBSCRIPTIONS_STANDARD=ON -DMICRO_OPCUA_BASE_TYPE_SYSTEM=ON -DMU_MAX_MONITORED_ITEMS=100 -DMU_MAX_SUBSCRIPTIONS=2 -DMU_MAX_PUBLISH_REQUESTS=5 -DMU_MONITORED_QUEUE_DEPTH=2 -DMU_MAX_TRIGGER_LINKS=4 -DCMAKE_C_COMPILER=clang` | PASS, exit code 0. CMake wrote build files to `build/fuzz`. Transcript: `/tmp/micro-opcua-t087b-fuzz-configure.log`. |
| Build target | `cmake --build build/fuzz --target fuzz_message_chunk` | PASS, exit code 0. Produced `build/fuzz/tests/fuzz/fuzz_message_chunk`. Transcript: `/tmp/micro-opcua-t087b-fuzz-message-chunk-build.log`. |
| Smoke | `build/fuzz/tests/fuzz/fuzz_message_chunk -runs=1` | PASS, exit code 0. libFuzzer completed `Done 2 runs in 0 second(s)` with no crash, timeout, OOM, or sanitizer finding reported. Transcript: `/tmp/micro-opcua-t087b-fuzz-message-chunk-smoke.log`. |
| T097c smoke refresh | `build/fuzz/tests/fuzz/fuzz_message_chunk -runs=1` | PASS, exit code 0. Used existing binary `build/fuzz/tests/fuzz/fuzz_message_chunk`; prerequisites were not rerun because the binary existed and was newer than checked fuzz source/CMake inputs. No corpus or fixture was provided, so libFuzzer started from an empty corpus and completed `Done 2 runs in 0 second(s)` with no crash, timeout, OOM, or sanitizer finding reported. Transcript: `/tmp/micro-opcua-t097c/fuzz-message-chunk-smoke.stdout-stderr.log`; exit code file: `/tmp/micro-opcua-t097c/fuzz-message-chunk-smoke.exitcode`. |
| Fixed-input smoke | `build/fuzz/tests/fuzz/fuzz_message_chunk -runs=1 tests/fixtures/uasc/msg_chunk.bin` | PASS, exit code 0. libFuzzer executed `tests/fixtures/uasc/msg_chunk.bin` once with no crash, timeout, OOM, or sanitizer finding reported; libFuzzer noted this was fixed-input execution, not fuzzing. Transcript: `/tmp/micro-opcua-t087b-fuzz-message-chunk-fixed-input.log`. |

### T087c / US4: `fuzz_activate_session`

Coverage and release-gate intent:

- Requirements: FR-003, FR-005, FR-008, FR-020.
- Success criteria: SC-003, SC-006.
- Normative scope: OPC-10000-4 sections 5.7.3 ActivateSession and 7.40.2.1
  user identity tokens; OPC-10000-6 section 5.2.2.15 ExtensionObject.
- Target/source mapping: `tests/fuzz/CMakeLists.txt` defines
  `fuzz_activate_session` from `tests/fuzz/fuzz_activate_session.c`.
- Harness behavior checked: raw and structured ActivateSession request bodies
  must keep identity-token ExtensionObject, locale/software-certificate array,
  signature, session-token, and credential-policy decode paths bounded; malformed
  or overdeclared data must not crash, time out, OOM, or produce sanitizer
  findings. This evidence complements the host evidence that plaintext
  username/password over `SecurityPolicy#None` is not advertised or accepted by
  default; it does not independently prove those policy tests.
- Gate honesty: this row records short libFuzzer smokes for the ActivateSession
  target only, including the fixed-input smoke named by
  `docs/validation/audit-hardening.md`. Full Feature 020 fuzz-gate status also
  depends on the other target rows below.

Commands and results:

| Step | Command | Result |
| --- | --- | --- |
| Configure | `cmake -S . -B build/fuzz -DMICRO_OPCUA_BUILD_FUZZERS=ON -DMICRO_OPCUA_SUBSCRIPTIONS_STANDARD=ON -DMICRO_OPCUA_BASE_TYPE_SYSTEM=ON -DMU_MAX_MONITORED_ITEMS=100 -DMU_MAX_SUBSCRIPTIONS=2 -DMU_MAX_PUBLISH_REQUESTS=5 -DMU_MONITORED_QUEUE_DEPTH=2 -DMU_MAX_TRIGGER_LINKS=4 -DCMAKE_C_COMPILER=clang` | PASS, exit code 0. CMake wrote build files to `build/fuzz`; Clang available as Ubuntu clang 18.1.3. Transcript: `/tmp/micro-opcua-t087c-fuzz-configure.log`. |
| Build target | `cmake --build build/fuzz --target fuzz_activate_session` | PASS, exit code 0. Produced `build/fuzz/tests/fuzz/fuzz_activate_session`. Transcript: `/tmp/micro-opcua-t087c-fuzz-activate-session-build.log`. |
| Smoke | `build/fuzz/tests/fuzz/fuzz_activate_session -runs=1` | PASS, exit code 0. libFuzzer completed `Done 2 runs in 0 second(s)` with no crash, timeout, OOM, or sanitizer finding reported. Transcript: `/tmp/micro-opcua-t087c-fuzz-activate-session-smoke.log`. |
| T097e smoke refresh | `build/fuzz/tests/fuzz/fuzz_activate_session -runs=1` | PASS, exit code 0. Used existing binary `build/fuzz/tests/fuzz/fuzz_activate_session`; prerequisites were not rerun because the binary existed and was newer than checked fuzz source/CMake inputs. No corpus or fixture was provided, so libFuzzer started from an empty corpus and completed `Done 2 runs in 0 second(s)` with no crash, timeout, OOM, or sanitizer finding reported. Transcript: `/tmp/micro-opcua-t097e/fuzz-activate-session-smoke.stdout-stderr.log`; exit code file: `/tmp/micro-opcua-t097e/fuzz-activate-session-smoke.exitcode`. |
| Fixed-input smoke | `build/fuzz/tests/fuzz/fuzz_activate_session -runs=1 tests/fixtures/services/activatesession_req.bin` | PASS, exit code 0. libFuzzer executed `tests/fixtures/services/activatesession_req.bin` once with no crash, timeout, OOM, or sanitizer finding reported; libFuzzer noted this was fixed-input execution, not fuzzing. Transcript: `/tmp/micro-opcua-t087c-fuzz-activate-session-fixed-input.log`. |

### T087d / US4: `fuzz_binary_types`

Coverage and release-gate intent:

- Requirements: FR-001, FR-003, FR-020.
- Success criteria: SC-001, SC-006.
- Normative scope: OPC-10000-6 section 5.2 binary encoding, OPC-10000-6
  section 5.2.2.15 ExtensionObject, and OPC-10000-6 section 5.2.5 Arrays.
- Target/source mapping: `tests/fuzz/CMakeLists.txt` defines
  `fuzz_binary_types` from `tests/fuzz/fuzz_binary_types.c`.
- Harness behavior: NodeId, String, ByteString, ExtensionObject,
  Variant, DataValue, and array decode surfaces should consume fuzz input and
  keep malformed lengths, array counts below `-1`, and overdeclared bodies from
  crashing, timing out, OOMing, producing sanitizer findings, or silently
  bypassing `Bad_DecodingError` coverage where applicable.
- Gate honesty: T087d originally recorded a placeholder/input-discard blocker.
  T102b replaced that target with input-consuming coverage for the listed binary
  types. The current short smoke is not a long fuzz campaign, but it clears the
  placeholder blocker and exercises more than startup-only coverage.

Commands and results:

| Step | Command | Result |
| --- | --- | --- |
| Placeholder scan | `rg -n "To be implemented|TODO|\\(void\\)data;|\\(void\\)size;" tests/fuzz/fuzz_binary_types.c` | Historical T087d blocker: exit code `0` before remediation. T102b refresh: no placeholder/input-discard marker remains in `tests/fuzz/fuzz_binary_types.c`; the expected no-match result is exit code `1`. |
| Configure | `cmake -S . -B build/fuzz -DMICRO_OPCUA_BUILD_FUZZERS=ON -DMICRO_OPCUA_SUBSCRIPTIONS_STANDARD=ON -DMICRO_OPCUA_BASE_TYPE_SYSTEM=ON -DMU_MAX_MONITORED_ITEMS=100 -DMU_MAX_SUBSCRIPTIONS=2 -DMU_MAX_PUBLISH_REQUESTS=5 -DMU_MONITORED_QUEUE_DEPTH=2 -DMU_MAX_TRIGGER_LINKS=4 -DCMAKE_C_COMPILER=clang` | PASS, exit code 0. CMake wrote build files to `build/fuzz`; Clang available as Ubuntu clang 18.1.3. Transcript: `/tmp/micro-opcua-t087d-fuzz-configure.log`. |
| Build target | `cmake --build build/fuzz --target fuzz_binary_types` | PASS, exit code 0. Produced `build/fuzz/tests/fuzz/fuzz_binary_types`. Transcript: `/tmp/micro-opcua-t087d-fuzz-binary-types-build.log`. |
| Smoke | `build-fuzz/tests/fuzz/fuzz_binary_types -runs=2` | PASS, exit code `0`. The T102b libFuzzer smoke completed with no crash, timeout, OOM, or sanitizer finding and reported non-startup coverage (`cov: 11`, `ft: 12`). |
| Conformance-doc blocker check | `cmake --build build/test --target test_conformance_docs && ctest --test-dir build/test -R '^test_conformance_docs$' --output-on-failure` | PASS. The doc gate that rejects fuzz TODO placeholder markers and input-discard harnesses passed after the T102b `fuzz_binary_types` replacement. |

### T087e / US4: `fuzz_subscription_filters`

Coverage and release-gate intent:

- Requirements: FR-013, FR-017, FR-020.
- Success criteria: SC-004, SC-006.
- Normative scope: OPC-10000-4 sections 5.13.2.4 and 5.13.3.4
  MonitoredItem filter result StatusCodes, OPC-10000-4 section 7.7.1
  ContentFilter count/operand structure, and OPC-10000-4 section 7.22.1
  MonitoringFilter.
- Target/source mapping: `tests/fuzz/CMakeLists.txt` defines
  `fuzz_subscription_filters` from
  `tests/fuzz/fuzz_subscription_filters.c`.
- Harness behavior checked: raw fuzz bytes are dispatched through the
  subscription filter request paths so malformed MonitoringFilter bodies,
  unsupported/invalid/disallowed filter encodings, and count/operand boundary
  inputs remain crash-free under libFuzzer. Exact per-item StatusCode semantics
  for unsupported, invalid, and disallowed filters remain evidence-linked to
  the host `test_subscriptions_errors` row in
  `docs/validation/audit-hardening.md`; this smoke does not independently prove
  every `ModifyMonitoredItems` StatusCode branch.
- Gate honesty: this row records a short libFuzzer smoke for the subscription
  filter target only, matching the active fuzz command intent in
  `docs/validation/audit-hardening.md`. Full Feature 020 fuzz-gate status is the
  aggregate of this row and the other target rows, including the T102b
  `fuzz_binary_types` refresh.

Commands and results:

| Step | Command | Result |
| --- | --- | --- |
| Configure | `cmake -S . -B build/fuzz -DMICRO_OPCUA_BUILD_FUZZERS=ON -DMICRO_OPCUA_SUBSCRIPTIONS_STANDARD=ON -DMICRO_OPCUA_BASE_TYPE_SYSTEM=ON -DMU_MAX_MONITORED_ITEMS=100 -DMU_MAX_SUBSCRIPTIONS=2 -DMU_MAX_PUBLISH_REQUESTS=5 -DMU_MONITORED_QUEUE_DEPTH=2 -DMU_MAX_TRIGGER_LINKS=4 -DCMAKE_C_COMPILER=clang` | PASS, exit code 0. CMake wrote build files to `build/fuzz`; Clang available as Ubuntu clang 18.1.3. Transcript: `/tmp/micro-opcua-t087e-fuzz-configure.log`. |
| Build target | `cmake --build build/fuzz --target fuzz_subscription_filters` | PASS, exit code 0. Produced executable `build/fuzz/tests/fuzz/fuzz_subscription_filters`. Transcript: `/tmp/micro-opcua-t087e-fuzz-subscription-filters-build.log`. |
| Smoke | `build/fuzz/tests/fuzz/fuzz_subscription_filters -runs=1` | PASS, exit code 0. libFuzzer completed `Done 2 runs in 0 second(s)` with no crash, timeout, OOM, or sanitizer finding reported; run reported `cov: 8`, `ft: 9`, `corp: 1/1b`, and `rss: 29Mb`. Transcript: `/tmp/micro-opcua-t087e-fuzz-subscription-filters-smoke.log`. |
| T097i smoke refresh | `build/fuzz/tests/fuzz/fuzz_subscription_filters -runs=1` | PASS, exit code 0. Used existing binary `build/fuzz/tests/fuzz/fuzz_subscription_filters`; prerequisites were not rerun because the binary existed and was newer than checked fuzz/CMake inputs. No input, corpus, or fixture was provided, so libFuzzer started from an empty corpus and completed `Done 2 runs in 0 second(s)` with `cov: 8`, `ft: 9`, `corp: 1/1b`, and `rss: 29Mb`; no crash, timeout, OOM, or sanitizer finding was reported. Transcript: `/tmp/micro-opcua-t097i/fuzz-subscription-filters-smoke.stdout-stderr.log`; exit code file: `/tmp/micro-opcua-t097i/fuzz-subscription-filters-smoke.exitcode`. |

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
