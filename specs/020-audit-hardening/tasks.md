# Tasks: Audit Hardening

**Input**: Design documents from `/specs/020-audit-hardening/`
**Prerequisites**: plan.md, spec.md, research.md, data-model.md, contracts/

**Tests**: Mandatory for every protocol parsing, serialization, dispatch, StatusCode, SecureChannel/session, address-space, security, and wire-visible change. Test tasks must be run before their matching implementation tasks and should fail before implementation.

**Organization**: Tasks are grouped by user story to keep each story independently testable. Shared decoder, status, and transport foundations come first because later story tasks depend on them. No tasks are marked `[P]`; the list is intentionally serialized because many atomic tasks touch shared test and source files.

## Resource-Risk Markers

- `[Resource: Flash]`: May increase code size or runtime tables.
- `[Resource: RAM]`: May add persistent/static storage, buffers, queues, or larger state.
- `[Resource: Stack]`: May increase stack use or local temporary storage.
- `[Resource: Throughput]`: May add checks to hot parsing, dispatch, transport, or service paths.

## Phase 1: Setup (Shared Evidence and Traceability)

**Purpose**: Establish evidence surfaces before implementation starts.

- [X] T001a Record the OPC UA MCP citation query for OPC-10000-4 section 5.5.2.2 FindServers in docs/traceability/mcp-query-ledger.md
- [X] T001b Record the OPC UA MCP citation query for OPC-10000-4 section 5.5.4.2 GetEndpoints in docs/traceability/mcp-query-ledger.md
- [X] T001c Record the OPC UA MCP citation query for OPC-10000-4 section 5.6.2 OpenSecureChannel in docs/traceability/mcp-query-ledger.md
- [X] T001d Record the OPC UA MCP citation query for OPC-10000-4 section 5.7.2 CreateSession in docs/traceability/mcp-query-ledger.md
- [X] T001e Record the OPC UA MCP citation query for OPC-10000-4 section 5.7.3 ActivateSession in docs/traceability/mcp-query-ledger.md
- [X] T001f Record the OPC UA MCP citation query for OPC-10000-4 section 5.9.2.4 Browse StatusCodes in docs/traceability/mcp-query-ledger.md
- [X] T001g Record the OPC UA MCP citation query for OPC-10000-4 section 5.11.2.3 Read service results in docs/traceability/mcp-query-ledger.md
- [X] T001h Record the OPC UA MCP citation query for OPC-10000-4 section 5.11.3.2 HistoryRead parameters in docs/traceability/mcp-query-ledger.md
- [X] T001i Record the OPC UA MCP citation query for OPC-10000-4 section 5.11.4.2 Write parameters in docs/traceability/mcp-query-ledger.md
- [X] T001j Record the OPC UA MCP citation query for OPC-10000-4 section 5.13.2.4 CreateMonitoredItems StatusCodes in docs/traceability/mcp-query-ledger.md
- [X] T001k Record the OPC UA MCP citation query for OPC-10000-4 section 5.13.3.4 ModifyMonitoredItems StatusCodes in docs/traceability/mcp-query-ledger.md
- [X] T001l Record the OPC UA MCP citation query for OPC-10000-4 section 5.14.5 Publish in docs/traceability/mcp-query-ledger.md
- [X] T001m Record the OPC UA MCP citation query for OPC-10000-4 section 7.22.1 MonitoringFilter in docs/traceability/mcp-query-ledger.md
- [X] T001n Record the OPC UA MCP citation query for OPC-10000-4 section 7.22.4 AggregateFilter in docs/traceability/mcp-query-ledger.md
- [X] T001o Record the OPC UA MCP citation query for OPC-10000-4 section 7.38.2 Common StatusCodes in docs/traceability/mcp-query-ledger.md
- [X] T001p Record the OPC UA MCP citation query for OPC-10000-4 section 7.39 TimestampsToReturn in docs/traceability/mcp-query-ledger.md
- [X] T001q Record the OPC UA MCP citation query for OPC-10000-6 section 5.2.5 Arrays in docs/traceability/mcp-query-ledger.md
- [X] T001r Record the OPC UA MCP citation query for OPC-10000-6 section 5.2.2.15 ExtensionObject in docs/traceability/mcp-query-ledger.md
- [X] T001s Record the OPC UA MCP citation query for OPC-10000-6 section 6.7.2 MessageChunk in docs/traceability/mcp-query-ledger.md
- [X] T001t Record the OPC UA MCP citation query for OPC-10000-6 section 7.1.2.2 Message Header in docs/traceability/mcp-query-ledger.md
- [X] T001u Record the OPC UA MCP citation query for OPC-10000-6 section 7.1.2.3 Hello Message in docs/traceability/mcp-query-ledger.md
- [X] T001v Record the OPC UA MCP citation query for OPC-10000-6 section 7.1.2.4 Acknowledge Message in docs/traceability/mcp-query-ledger.md
- [X] T001w Record the OPC UA MCP citation query for OPC-10000-7 section 4.2 Conformance Units in docs/traceability/mcp-query-ledger.md
- [X] T001x Record the OPC UA MCP citation query for OPC-10000-7 section 4.3 Profiles in docs/traceability/mcp-query-ledger.md
- [X] T002a Create the FR-to-OPC-section traceability skeleton in docs/traceability/020-audit-hardening.md
- [X] T002b Create the FR-to-target-test traceability skeleton in docs/traceability/020-audit-hardening.md
- [X] T003a Create the audit closure matrix skeleton in docs/validation/audit-hardening-closure.md
- [X] T003b Import the authoritative six-agent triage finding IDs, source audit lanes, and severities into docs/validation/audit-hardening-closure.md
- [X] T004a Run the pre-change size-report baseline command referenced by docs/size/audit-hardening-baseline.md
- [X] T004b Record the pre-change size-report baseline in docs/size/audit-hardening-baseline.md
- [X] T004c Run the pre-change stack-usage baseline command referenced by docs/size/audit-hardening-baseline.md
- [X] T004d Record the pre-change stack-usage baseline in docs/size/audit-hardening-baseline.md
- [X] T005a Create the host-test validation checklist in docs/validation/audit-hardening.md
- [X] T005b Create the sanitizer validation checklist in docs/validation/audit-hardening.md
- [X] T005c Create the fuzz validation checklist in docs/validation/audit-hardening.md
- [X] T005d Create the embedded-build validation checklist in docs/validation/audit-hardening.md
- [X] T005e Create the conformance-doc validation checklist in docs/validation/audit-hardening.md
- [X] T005f Create the size-budget validation checklist in docs/validation/audit-hardening.md
- [X] T005g Create the formatting/static-analysis validation checklist in docs/validation/audit-hardening.md
- [X] T005h Create the latency-threshold validation checklist with the 5% comparison rule in docs/validation/audit-hardening.md
- [X] T005h1 [Resource: Flash/Throughput] Add the host latency benchmark command for the valid minimal discovery/session/read flow referenced by docs/validation/audit-hardening.md
- [X] T005i Run the pre-change valid minimal discovery/session/read latency baseline command referenced by docs/validation/audit-hardening.md
- [X] T005j Record the pre-change valid minimal discovery/session/read latency baseline in docs/validation/audit-hardening.md
- [X] T006a Add the malformed-array fixture index in tests/fixtures/audit-hardening/README.md
- [X] T006b Add the malformed-ExtensionObject fixture index in tests/fixtures/audit-hardening/README.md
- [X] T006c Add the malformed-MessageChunk fixture index in tests/fixtures/audit-hardening/README.md
- [X] T006d Add the malformed-Session fixture index in tests/fixtures/audit-hardening/README.md
- [X] T006e Add the malformed-filter fixture index in tests/fixtures/audit-hardening/README.md
- [X] T006f Add the truncated-service-body fixture index in tests/fixtures/audit-hardening/README.md

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Shared status, decoder, transport, and dispatch behavior required by all user stories.

**CRITICAL**: No user story implementation can begin until this phase is complete.

- [X] T007a Add the exact numeric StatusCode regression test for Bad_DecodingError citing OPC-10000-4 section 7.38.2 in tests/unit/test_status.c
- [X] T008a Implement the public Bad_DecodingError StatusCode constant citing OPC-10000-4 section 7.38.2 in include/micro_opcua/status.h
- [X] T009a [Resource: Flash] Implement the Bad_DecodingError status mapping citing OPC-10000-4 section 7.38.2 in src/core/status.c
- [X] T007b Add the exact numeric StatusCode regression test for Bad_MonitoredItemFilterUnsupported citing OPC-10000-4 section 7.38.2 in tests/unit/test_status.c
- [X] T008b Implement the public Bad_MonitoredItemFilterUnsupported StatusCode constant citing OPC-10000-4 section 7.38.2 in include/micro_opcua/status.h
- [X] T009b [Resource: Flash] Implement the Bad_MonitoredItemFilterUnsupported status mapping citing OPC-10000-4 section 7.38.2 in src/core/status.c
- [X] T007c Add the exact numeric StatusCode regression test for Bad_MonitoredItemFilterInvalid citing OPC-10000-4 section 7.38.2 in tests/unit/test_status.c
- [X] T008c Implement the public Bad_MonitoredItemFilterInvalid StatusCode constant citing OPC-10000-4 section 7.38.2 in include/micro_opcua/status.h
- [X] T009c [Resource: Flash] Implement the Bad_MonitoredItemFilterInvalid status mapping citing OPC-10000-4 section 7.38.2 in src/core/status.c
- [X] T007d Add the exact numeric StatusCode regression test for Bad_FilterNotAllowed citing OPC-10000-4 section 7.38.2 in tests/unit/test_status.c
- [X] T008d Implement the public Bad_FilterNotAllowed StatusCode constant citing OPC-10000-4 section 7.38.2 in include/micro_opcua/status.h
- [X] T009d [Resource: Flash] Implement the Bad_FilterNotAllowed status mapping citing OPC-10000-4 section 7.38.2 in src/core/status.c
- [X] T007e Add the exact numeric StatusCode regression test for Bad_TimestampsToReturnInvalid citing OPC-10000-4 section 7.38.2 in tests/unit/test_status.c
- [X] T008e Implement the public Bad_TimestampsToReturnInvalid StatusCode constant citing OPC-10000-4 section 7.38.2 in include/micro_opcua/status.h
- [X] T009e [Resource: Flash] Implement the Bad_TimestampsToReturnInvalid status mapping citing OPC-10000-4 section 7.38.2 in src/core/status.c
- [X] T007f Add the exact numeric StatusCode regression test for Bad_BrowseDirectionInvalid citing OPC-10000-4 section 7.38.2 in tests/unit/test_status.c
- [X] T008f Implement the public Bad_BrowseDirectionInvalid StatusCode constant citing OPC-10000-4 section 7.38.2 in include/micro_opcua/status.h
- [X] T009f [Resource: Flash] Implement the Bad_BrowseDirectionInvalid status mapping citing OPC-10000-4 section 7.38.2 in src/core/status.c
- [X] T007g Add the exact numeric StatusCode regression test for Bad_SessionNotActivated citing OPC-10000-4 section 7.38.2 in tests/unit/test_status.c
- [X] T008g Implement the public Bad_SessionNotActivated StatusCode constant citing OPC-10000-4 section 7.38.2 in include/micro_opcua/status.h
- [X] T009g [Resource: Flash] Implement the Bad_SessionNotActivated status mapping citing OPC-10000-4 section 7.38.2 in src/core/status.c
- [X] T007h Add the exact numeric StatusCode regression test for Bad_SessionIdInvalid citing OPC-10000-4 section 7.38.2 in tests/unit/test_status.c
- [X] T008h Implement the public Bad_SessionIdInvalid StatusCode constant citing OPC-10000-4 section 7.38.2 in include/micro_opcua/status.h
- [X] T009h [Resource: Flash] Implement the Bad_SessionIdInvalid status mapping citing OPC-10000-4 section 7.38.2 in src/core/status.c
- [X] T007i Add the exact numeric StatusCode regression test for Bad_SecureChannelIdInvalid citing OPC-10000-4 section 7.38.2 in tests/unit/test_status.c
- [X] T008i Implement the public Bad_SecureChannelIdInvalid StatusCode constant citing OPC-10000-4 section 7.38.2 in include/micro_opcua/status.h
- [X] T009i [Resource: Flash] Implement the Bad_SecureChannelIdInvalid status mapping citing OPC-10000-4 section 7.38.2 in src/core/status.c
- [X] T007j Add the exact numeric StatusCode regression test for Bad_SecurityChecksFailed citing OPC-10000-4 section 7.38.2 in tests/unit/test_status.c
- [X] T008j Implement the public Bad_SecurityChecksFailed StatusCode constant citing OPC-10000-4 section 7.38.2 in include/micro_opcua/status.h
- [X] T009j [Resource: Flash] Implement the Bad_SecurityChecksFailed status mapping citing OPC-10000-4 section 7.38.2 in src/core/status.c
- [X] T007k Add the exact numeric StatusCode regression test for Bad_IdentityTokenRejected citing OPC-10000-4 section 7.38.2 in tests/unit/test_status.c
- [X] T008k Implement the public Bad_IdentityTokenRejected StatusCode constant citing OPC-10000-4 section 7.38.2 in include/micro_opcua/status.h
- [X] T009k [Resource: Flash] Implement the Bad_IdentityTokenRejected status mapping citing OPC-10000-4 section 7.38.2 in src/core/status.c
- [X] T007l Add the exact numeric StatusCode regression test for Bad_ResponseTooLarge citing OPC-10000-4 section 7.38.2 in tests/unit/test_status.c
- [X] T008l Implement the public Bad_ResponseTooLarge StatusCode constant citing OPC-10000-4 section 7.38.2 in include/micro_opcua/status.h
- [X] T009l [Resource: Flash] Implement the Bad_ResponseTooLarge status mapping citing OPC-10000-4 section 7.38.2 in src/core/status.c
- [X] T007m Add the exact numeric StatusCode regression test for Bad_TcpMessageTypeInvalid citing OPC-10000-4 section 7.38.2 in tests/unit/test_status.c
- [X] T008m Implement the public Bad_TcpMessageTypeInvalid StatusCode constant citing OPC-10000-4 section 7.38.2 in include/micro_opcua/status.h
- [X] T009m [Resource: Flash] Implement the Bad_TcpMessageTypeInvalid status mapping citing OPC-10000-4 section 7.38.2 in src/core/status.c
- [X] T007n Add the exact numeric StatusCode regression test for Bad_ServiceUnsupported citing OPC-10000-4 section 7.38.2 in tests/unit/test_status.c
- [X] T008n Implement the public Bad_ServiceUnsupported StatusCode constant citing OPC-10000-4 section 7.38.2 in include/micro_opcua/status.h
- [X] T009n [Resource: Flash] Implement the Bad_ServiceUnsupported status mapping citing OPC-10000-4 section 7.38.2 in src/core/status.c
- [X] T007o Add the exact numeric StatusCode regression test for Bad_TooManyOperations citing OPC-10000-4 section 7.38.2 in tests/unit/test_status.c
- [X] T008o Implement the public Bad_TooManyOperations StatusCode constant citing OPC-10000-4 section 7.38.2 in include/micro_opcua/status.h
- [X] T009o [Resource: Flash] Implement the Bad_TooManyOperations status mapping citing OPC-10000-4 section 7.38.2 in src/core/status.c
- [X] T007p Add the exact numeric StatusCode regression test for Bad_TypeMismatch citing OPC-10000-4 section 7.38.2 in tests/unit/test_status.c
- [X] T008p Implement the public Bad_TypeMismatch StatusCode constant citing OPC-10000-4 section 7.38.2 in include/micro_opcua/status.h
- [X] T009p [Resource: Flash] Implement the Bad_TypeMismatch status mapping citing OPC-10000-4 section 7.38.2 in src/core/status.c
- [X] T007q Add the exact numeric StatusCode regression test for Bad_ContinuationPointInvalid citing OPC-10000-4 section 7.38.2 in tests/unit/test_status.c
- [X] T008q Implement the public Bad_ContinuationPointInvalid StatusCode constant citing OPC-10000-4 section 7.38.2 in include/micro_opcua/status.h
- [X] T009q [Resource: Flash] Implement the Bad_ContinuationPointInvalid status mapping citing OPC-10000-4 section 7.38.2 in src/core/status.c
- [X] T007r Add the exact numeric StatusCode regression test for Bad_EncodingLimitsExceeded citing OPC-10000-4 section 7.38.2 in tests/unit/test_status.c
- [X] T008r Implement the public Bad_EncodingLimitsExceeded StatusCode constant citing OPC-10000-4 section 7.38.2 in include/micro_opcua/status.h
- [X] T009r [Resource: Flash] Implement the Bad_EncodingLimitsExceeded status mapping citing OPC-10000-4 section 7.38.2 in src/core/status.c
- [X] T010a Add the null-array length -1 test per OPC-10000-6 section 5.2.5 in tests/unit/test_binary_array_errors.c
- [X] T010b Add the empty-array length 0 test per OPC-10000-6 section 5.2.5 in tests/unit/test_binary_array_errors.c
- [X] T010c Add the positive-array-count decode test per OPC-10000-6 section 5.2.5 in tests/unit/test_binary_array_errors.c
- [X] T010d Add the array-count-below-minus-one Bad_DecodingError test per OPC-10000-6 section 5.2.5 in tests/unit/test_binary_array_errors.c
- [X] T011a [Resource: Throughput] Implement null-array length -1 handling per OPC-10000-6 section 5.2.5 in src/encoding/binary_reader.c
- [X] T011b [Resource: Throughput] Implement array-count-below-minus-one rejection per OPC-10000-6 section 5.2.5 in src/encoding/binary_reader.c
- [X] T011c [Resource: Throughput] Implement positive-array-count overflow rejection per OPC-10000-6 section 5.2.5 in src/encoding/binary_reader.c
- [X] T012a Add the ExtensionObject declared-body overrun Bad_DecodingError test per OPC-10000-6 section 5.2.2.15 in tests/unit/test_binary_extension_object_errors.c
- [X] T012b Add the ExtensionObject declared-body underrun Bad_DecodingError test per OPC-10000-6 section 5.2.2.15 in tests/unit/test_binary_extension_object_errors.c
- [X] T013a [Resource: Throughput] Implement bounded ExtensionObject subreader creation per OPC-10000-6 section 5.2.2.15 in src/encoding/binary_extension_object.c
- [X] T013b [Resource: Throughput] Implement bounded ExtensionObject body skip behavior per OPC-10000-6 section 5.2.2.15 in src/encoding/binary_extension_object.c
- [X] T014a Add the overdeclared String length Bad_DecodingError test per OPC-10000-6 section 5.2 in tests/unit/test_binary_string_errors.c
- [X] T014b Add the overdeclared ByteString length Bad_DecodingError test per OPC-10000-6 section 5.2 in tests/unit/test_binary_string_errors.c
- [X] T014c Add the reader-position overflow Bad_DecodingError test per OPC-10000-6 section 5.2 in tests/unit/test_binary_string_errors.c
- [X] T015a [Resource: Throughput] Implement overflow-safe declared-length validation per OPC-10000-6 section 5.2 in src/encoding/binary_reader.c
- [X] T015b [Resource: Throughput] Implement remaining-byte validation per OPC-10000-6 section 5.2 in src/encoding/binary_reader.c
- [X] T015c [Resource: Throughput] Implement overflow-safe skip validation per OPC-10000-6 section 5.2 in src/encoding/binary_reader.c
- [X] T016a Add the truncated service body no-callback test expecting Bad_DecodingError per OPC-10000-4 section 7.38.2 in tests/unit/test_dispatch_services.c
- [X] T016b Add the truncated service body no-state-mutation test expecting Bad_DecodingError per OPC-10000-4 section 7.38.2 in tests/unit/test_dispatch_services.c
- [X] T017 [Resource: Throughput] Implement mandatory service-body completeness enforcement before dispatch per OPC-10000-4 section 7.38.2 in src/core/service_dispatch.c
- [X] T018a Add the invalid TCP message type Bad_TcpMessageTypeInvalid test per OPC-10000-6 sections 6.7.2 and 7.1.2.2 in tests/unit/test_message_chunk_errors.c
- [X] T018b Add the abort chunk TCP Error no-dispatch test per OPC-10000-6 section 6.7.2 in tests/unit/test_message_chunk_errors.c
- [X] T018c Add the non-final chunk TCP Error no-dispatch test per OPC-10000-6 section 6.7.2 in tests/unit/test_message_chunk_errors.c
- [X] T019a [Resource: Throughput] Implement invalid TCP message type rejection per OPC-10000-6 sections 6.7.2 and 7.1.2.2 in src/core/message_chunk.c
- [X] T019b [Resource: Throughput] Implement abort chunk no-dispatch handling per OPC-10000-6 section 6.7.2 in src/core/message_chunk.c
- [X] T019c [Resource: Throughput] Implement non-final chunk no-dispatch handling in single-chunk mode per OPC-10000-6 section 6.7.2 in src/core/message_chunk.c
- [X] T020a Add the ACK SendBufferSize cap test per OPC-10000-6 section 7.1.2.4 in tests/unit/test_tcp_connection.c
- [X] T020b Add the unsupported Hello buffer-size TCP Error test per OPC-10000-6 sections 7.1.2.3 and 7.1.2.4 in tests/unit/test_tcp_connection.c
- [X] T021a [Resource: Throughput] Implement ACK SendBufferSize capping per OPC-10000-6 section 7.1.2.4 in src/core/tcp_connection.c
- [X] T021b [Resource: Throughput] Implement deterministic rejection for unsupported Hello buffer sizes per OPC-10000-6 sections 7.1.2.3 and 7.1.2.4 in src/core/tcp_connection.c
- [X] T022a Extend binary reader fuzz coverage for array counts below -1 per OPC-10000-6 section 5.2.5 in tests/fuzz/fuzz_binary_reader.c
- [X] T022b Extend binary reader fuzz coverage for declared length boundaries per OPC-10000-6 section 5.2 in tests/fuzz/fuzz_binary_reader.c
- [X] T023a Extend message chunk fuzz coverage for invalid message types per OPC-10000-6 section 7.1.2.2 in tests/fuzz/fuzz_message_chunk.c
- [X] T023b Extend message chunk fuzz coverage for abort chunks per OPC-10000-6 section 6.7.2 in tests/fuzz/fuzz_message_chunk.c
- [X] T023c Extend message chunk fuzz coverage for non-final chunks per OPC-10000-6 section 6.7.2 in tests/fuzz/fuzz_message_chunk.c
- [X] T023d Extend message chunk fuzz coverage for negotiated-size boundaries per OPC-10000-6 section 7.1.2.2 in tests/fuzz/fuzz_message_chunk.c

**Checkpoint**: Foundation ready. Exact statuses, decoder bounds, dispatch body checks, chunk validation, and ACK negotiation are independently testable.

---

## Phase 3: User Story 1 - Reject Malformed OPC UA Traffic Deterministically (Priority: P1) [MVP]

**Goal**: Malformed, truncated, or boundary-case traffic rejects with exact OPC UA StatusCodes or TCP Errors and does not mutate protocol state or call application handlers.

**Independent Test**: Run the US1 unit tests and dispatch malformed service requests for Discovery, Session, Read, Write, Browse, Subscription, Query, and History; verify cited StatusCodes and no state mutation.

### Tests for User Story 1

- [X] T024a [US1] Add the FindServers malformed array Bad_DecodingError test per OPC-10000-4 section 5.5.2.2 and OPC-10000-6 section 5.2.5 in tests/unit/test_discovery_services.c
- [X] T024b [US1] Add the FindServers truncated body Bad_DecodingError test per OPC-10000-4 section 5.5.2.2 in tests/unit/test_discovery_services.c
- [X] T024c [US1] Add the GetEndpoints malformed array Bad_DecodingError test per OPC-10000-4 section 5.5.4.2 and OPC-10000-6 section 5.2.5 in tests/unit/test_discovery_services.c
- [X] T024d [US1] Add the GetEndpoints truncated body Bad_DecodingError test per OPC-10000-4 section 5.5.4.2 in tests/unit/test_discovery_services.c
- [X] T025 [US1] Add the CreateSession truncation no-session-allocation Bad_DecodingError test per OPC-10000-4 section 5.7.2 in tests/unit/test_session.c
- [X] T026 [US1] Add the ActivateSession user identity ExtensionObject boundary Bad_DecodingError test per OPC-10000-4 section 5.7.3 and OPC-10000-6 section 5.2.2.15 in tests/unit/test_security_identity_errors.c
- [X] T027 [US1] Add the Read invalid TimestampsToReturn Bad_TimestampsToReturnInvalid test per OPC-10000-4 sections 5.11.2.3, 7.38.2, and 7.39 in tests/unit/test_read_service.c
- [X] T028 [US1] Add the Browse invalid BrowseDirection Bad_BrowseDirectionInvalid test per OPC-10000-4 sections 5.9.2.4 and 7.38.2 in tests/unit/test_browse_service.c
- [X] T029 [US1] Add the Write DataValue-without-Value no-callback Bad_TypeMismatch test per OPC-10000-4 sections 5.11.4.2 and 5.11.4.4 in tests/unit/test_write_errors.c
- [X] T030a [US1] Add the CreateMonitoredItems malformed filter-body Bad_DecodingError test per OPC-10000-4 section 5.13.2.4 in tests/unit/test_subscriptions_errors.c
- [X] T030b [US1] Add the ModifyMonitoredItems malformed filter-body Bad_DecodingError test per OPC-10000-4 section 5.13.3.4 in tests/unit/test_subscriptions_errors.c
- [X] T031a [US1] Add the QueryFirst ContentFilter count overflow Bad_DecodingError test per OPC-10000-4 sections B.2.3 and 7.7.1 in tests/unit/test_query_service.c
- [X] T031b [US1] Add the QueryFirst fixed-capacity Bad_TooManyOperations test per OPC-10000-4 sections B.2.3 and 7.7.1 in tests/unit/test_query_service.c
- [X] T032 [US1] Add the HistoryRead details ExtensionObject truncation Bad_DecodingError test per OPC-10000-4 section 5.11.3.2 and OPC-10000-6 section 5.2.2.15 in tests/unit/test_history.c

### Implementation for User Story 1

- [X] T033a [US1] [Resource: Throughput] Implement FindServers mandatory-body validation per OPC-10000-4 section 5.5.2.2 in src/services/discovery.c
- [X] T033b [US1] [Resource: Throughput] Implement FindServers array validation per OPC-10000-4 section 5.5.2.2 and OPC-10000-6 section 5.2.5 in src/services/discovery.c
- [X] T033c [US1] [Resource: Throughput] Implement GetEndpoints mandatory-body validation per OPC-10000-4 section 5.5.4.2 in src/services/discovery.c
- [X] T033d [US1] [Resource: Throughput] Implement GetEndpoints array validation per OPC-10000-4 section 5.5.4.2 and OPC-10000-6 section 5.2.5 in src/services/discovery.c
- [X] T034 [US1] [Resource: RAM/Throughput] Implement CreateSession full-body validation before session allocation per OPC-10000-4 section 5.7.2 in src/services/session.c
- [X] T035 [US1] [Resource: Throughput] Implement ActivateSession bounded user identity token decoding per OPC-10000-4 section 5.7.3 and OPC-10000-6 section 5.2.2.15 in src/services/session.c
- [X] T036 [US1] [Resource: Flash/Throughput] Implement Read TimestampsToReturn validation returning Bad_TimestampsToReturnInvalid per OPC-10000-4 sections 5.11.2.3, 7.38.2, and 7.39 in src/services/read.c
- [X] T037 [US1] [Resource: Flash/Throughput] Implement BrowseDirection validation returning Bad_BrowseDirectionInvalid per OPC-10000-4 sections 5.9.2.4 and 7.38.2 in src/services/browse.c
- [X] T038 [US1] [Resource: Throughput] Implement Write DataValue Value-presence guard returning Bad_TypeMismatch before application callbacks per OPC-10000-4 sections 5.11.4.2 and 5.11.4.4 in src/services/write.c
- [X] T039a [US1] [Resource: Throughput] Implement CreateMonitoredItems filter body completeness checks per OPC-10000-4 section 5.13.2.4 in src/services/subscription.c
- [X] T039b [US1] [Resource: Throughput] Implement ModifyMonitoredItems filter body completeness checks per OPC-10000-4 section 5.13.3.4 in src/services/subscription.c
- [X] T040a [US1] [Resource: RAM/Throughput] Implement QueryFirst ContentFilter count overflow rejection returning Bad_DecodingError per OPC-10000-4 sections B.2.3 and 7.7.1 in src/encoding/binary_query.c
- [X] T040b [US1] [Resource: RAM/Throughput] Implement QueryFirst fixed-capacity rejection returning Bad_TooManyOperations per OPC-10000-4 sections B.2.3 and 7.7.1 in src/encoding/binary_query.c
- [X] T041 [US1] [Resource: Throughput] Implement HistoryRead details ExtensionObject bounded decode per OPC-10000-4 section 5.11.3.2 and OPC-10000-6 section 5.2.2.15 in src/services/history.c
- [X] T042a [US1] Run the US1 Discovery unit tests referenced by docs/validation/audit-hardening.md
- [X] T042b [US1] Record the US1 Discovery test evidence in docs/validation/audit-hardening.md
- [X] T042c [US1] Run the US1 Session and identity unit tests referenced by docs/validation/audit-hardening.md
- [X] T042d [US1] Record the US1 Session and identity test evidence in docs/validation/audit-hardening.md
- [X] T042e [US1] Run the US1 Read unit tests referenced by docs/validation/audit-hardening.md
- [X] T042f [US1] Record the US1 Read test evidence in docs/validation/audit-hardening.md
- [X] T042g [US1] Run the US1 Write unit tests referenced by docs/validation/audit-hardening.md
- [X] T042h [US1] Record the US1 Write test evidence in docs/validation/audit-hardening.md
- [X] T042i [US1] Run the US1 Browse unit tests referenced by docs/validation/audit-hardening.md
- [X] T042j [US1] Record the US1 Browse test evidence in docs/validation/audit-hardening.md
- [X] T042k [US1] Run the US1 Subscription unit tests referenced by docs/validation/audit-hardening.md
- [X] T042l [US1] Record the US1 Subscription test evidence in docs/validation/audit-hardening.md
- [X] T042m [US1] Run the US1 Query unit tests referenced by docs/validation/audit-hardening.md
- [X] T042n [US1] Record the US1 Query test evidence in docs/validation/audit-hardening.md
- [X] T042o [US1] Run the US1 History unit tests referenced by docs/validation/audit-hardening.md
- [X] T042p [US1] Record the US1 History test evidence in docs/validation/audit-hardening.md

**Checkpoint**: US1 is independently testable and covers malformed-input rejection without state mutation.

---

## Phase 4: User Story 2 - Preserve Session, Channel, and Credential Security (Priority: P1)

**Goal**: Security-sensitive identifiers and nonces are fresh, sessions are bound to SecureChannels, unactivated sessions use the correct StatusCode, and plaintext username/password over SecurityPolicy None is disabled by default.

**Independent Test**: Run multi-channel and user-identity tests that attempt guessed tokens, entropy failure, cross-channel activation, unactivated service use, and plaintext username/password over None.

### Tests for User Story 2

- [X] T043 [US2] Add the OpenSecureChannel entropy-failure Bad_SecurityChecksFailed test per OPC-10000-4 sections 5.6.2.3 and 7.38.2 in tests/unit/test_secure_channel.c
- [X] T044a [US2] Add the CreateSession session-token freshness test per OPC-10000-4 section 5.7.2.2 in tests/unit/test_session.c
- [X] T044b [US2] Add the CreateSession server-nonce entropy-failure Bad_SecurityChecksFailed test per OPC-10000-4 sections 5.7.2.3 and 7.38.2 in tests/unit/test_session.c
- [X] T045a [US2] Add the cross-channel ActivateSession Bad_SecureChannelIdInvalid test per OPC-10000-4 sections 5.7.2.1 and 7.38.2 in tests/unit/test_connection_multiplex.c
- [X] T045b [US2] Add the cross-channel session-bound service Bad_SecureChannelIdInvalid test per OPC-10000-4 sections 5.7.2.1 and 7.38.2 in tests/unit/test_connection_multiplex.c
- [X] T046a [US2] Add the known inactive Session Bad_SessionNotActivated test per OPC-10000-4 section 7.38.2 in tests/unit/test_service_state_errors.c
- [X] T046b [US2] Add the unknown Session token Bad_SessionIdInvalid test per OPC-10000-4 section 7.38.2 in tests/unit/test_service_state_errors.c
- [X] T047a [US2] Add the SecurityPolicy None no-username-advertisement test per OPC-10000-4 sections 5.5.4.2 and 5.7.3 in tests/unit/test_security_identity_errors.c
- [X] T047b [US2] Add the SecurityPolicy None username activation Bad_IdentityTokenRejected test per OPC-10000-4 sections 5.7.3.3, 7.38.2, and 7.40.2.1 in tests/unit/test_security_identity_errors.c
- [X] T048a [US2] Add the anonymous-over-SecurityPolicy-None integration test per OPC-10000-4 section 5.5.4.2 in tests/integration/test_user_auth_plaintext.c
- [X] T048b [US2] Add the username-absent-over-SecurityPolicy-None integration test per OPC-10000-4 section 5.5.4.2 in tests/integration/test_user_auth_plaintext.c

### Implementation for User Story 2

- [X] T049 [US2] [Resource: Flash/Throughput] Implement OpenSecureChannel fail-closed entropy handling returning Bad_SecurityChecksFailed per OPC-10000-4 sections 5.6.2.3 and 7.38.2 in src/services/secure_channel.c
- [X] T050a [US2] [Resource: RAM/Throughput] Implement non-predictable Session identifier generation per OPC-10000-4 section 5.7.2.2 in src/services/session.c
- [X] T050b [US2] [Resource: RAM/Throughput] Implement non-predictable Session authentication token generation per OPC-10000-4 section 5.7.2.2 in src/services/session.c
- [X] T050c [US2] [Resource: RAM/Throughput] Implement server nonce freshness fail-closed behavior per OPC-10000-4 sections 5.7.2.3 and 7.38.2 in src/services/session.c
- [X] T051a [US2] [Resource: RAM/Throughput] Bind each Session to its creating SecureChannel per OPC-10000-4 section 5.7.2.1 in src/services/session.c
- [X] T051b [US2] [Resource: Throughput] Reject cross-channel ActivateSession with Bad_SecureChannelIdInvalid per OPC-10000-4 section 7.38.2 in src/services/session.c
- [X] T051c [US2] [Resource: Throughput] Reject cross-channel session-bound service use with Bad_SecureChannelIdInvalid per OPC-10000-4 section 7.38.2 in src/services/service_header.c
- [X] T052a [US2] [Resource: Flash/Throughput] Return Bad_SessionNotActivated for known inactive Sessions per OPC-10000-4 section 7.38.2 in src/services/service_header.c
- [X] T052b [US2] [Resource: Flash/Throughput] Return Bad_SessionIdInvalid for unknown Session tokens per OPC-10000-4 section 7.38.2 in src/services/service_header.c
- [X] T053 [US2] [Resource: Flash] Reject username/password user identity tokens over SecurityPolicy None by default per OPC-10000-4 sections 5.7.3.3 and 7.40.2.1 in src/security/security_policy.c
- [X] T054 [US2] [Resource: Flash] Remove username/password token advertisement from SecurityPolicy None endpoints per OPC-10000-4 section 5.5.4.2 in src/services/discovery.c
- [X] T055a [US2] Run the US2 SecureChannel unit tests referenced by docs/validation/audit-hardening.md
- [X] T055b [US2] Record the US2 SecureChannel test evidence in docs/validation/audit-hardening.md
- [X] T055c [US2] Run the US2 Session tests referenced by docs/validation/audit-hardening.md
- [X] T055d [US2] Record the US2 Session evidence in docs/validation/audit-hardening.md
- [X] T055e [US2] Run the US2 connection-multiplex tests referenced by docs/validation/audit-hardening.md
- [X] T055f [US2] Record the US2 connection-multiplex evidence in docs/validation/audit-hardening.md
- [X] T055g [US2] Run the US2 user-authentication integration tests referenced by docs/validation/audit-hardening.md
- [X] T055h [US2] Record the US2 user-authentication integration evidence in docs/validation/audit-hardening.md

**Checkpoint**: US2 is independently testable and proves secure defaults without broadening the security surface.

---

## Phase 5: User Story 3 - Match OPC UA Service Semantics for Claimed Features (Priority: P2)

**Goal**: Claimed Discovery, Attribute, Browse, Subscription, Aggregate, Query, History, Publish, and NodeManagement behavior either follows the cited OPC UA section or rejects unsupported options with the correct StatusCode.

**Independent Test**: Run service-specific negative and unsupported-option tests, then verify each response against the cited OPC UA section and conformance claim.

### Tests for User Story 3

- [X] T056a [US3] Add the FindServers endpoint URL filter test per OPC-10000-4 section 5.5.2.2 in tests/unit/test_discovery_services.c
- [X] T056b [US3] Add the FindServers locale filter test per OPC-10000-4 section 5.5.2.2 in tests/unit/test_discovery_services.c
- [X] T056c [US3] Add the FindServers server URI filter test per OPC-10000-4 section 5.5.2.2 in tests/unit/test_discovery_services.c
- [X] T056d [US3] Add the FindServers server type filter test per OPC-10000-4 section 5.5.2.2 in tests/unit/test_discovery_services.c
- [X] T057a [US3] Add the GetEndpoints endpoint URL filter test per OPC-10000-4 section 5.5.4.2 in tests/unit/test_discovery_endpoint.c
- [X] T057b [US3] Add the GetEndpoints locale filter test per OPC-10000-4 section 5.5.4.2 in tests/unit/test_discovery_endpoint.c
- [X] T057d [US3] Add the GetEndpoints transport profile filter test per OPC-10000-4 section 5.5.4.2 in tests/unit/test_discovery_endpoint.c
- [X] T058a [US3] Add the CreateMonitoredItems unsupported MonitoringFilter Bad_MonitoredItemFilterUnsupported test per OPC-10000-4 section 5.13.2.4 in tests/unit/test_subscriptions_errors.c
- [X] T058b [US3] Add the CreateMonitoredItems invalid MonitoringFilter Bad_MonitoredItemFilterInvalid test per OPC-10000-4 section 5.13.2.4 in tests/unit/test_subscriptions_errors.c
- [X] T058c [US3] Add the CreateMonitoredItems disallowed MonitoringFilter Bad_FilterNotAllowed test per OPC-10000-4 section 5.13.2.4 in tests/unit/test_subscriptions_errors.c
- [X] T058d [US3] Add the ModifyMonitoredItems unsupported MonitoringFilter Bad_MonitoredItemFilterUnsupported test per OPC-10000-4 section 5.13.3.4 in tests/unit/test_subscriptions_errors.c
- [X] T058e [US3] Add the ModifyMonitoredItems invalid MonitoringFilter Bad_MonitoredItemFilterInvalid test per OPC-10000-4 section 5.13.3.4 in tests/unit/test_subscriptions_errors.c
- [X] T058f [US3] Add the ModifyMonitoredItems disallowed MonitoringFilter Bad_FilterNotAllowed test per OPC-10000-4 section 5.13.3.4 in tests/unit/test_subscriptions_errors.c
- [X] T059a [US3] Add the AggregateFilter Average scoped-support test per OPC-10000-4 section 7.22.4 and OPC-10000-13 section 5.4.3.5 in tests/unit/test_aggregate.c
- [X] T059b [US3] Add the AggregateFilter Minimum scoped-support test per OPC-10000-4 section 7.22.4 and OPC-10000-13 section 5.4.3.10 in tests/unit/test_aggregate.c
- [X] T059c [US3] Add the AggregateFilter Maximum scoped-support test per OPC-10000-4 section 7.22.4 and OPC-10000-13 section 5.4.3.11 in tests/unit/test_aggregate.c
- [X] T059d [US3] Add the unsupported AggregateFilter Bad_MonitoredItemFilterUnsupported test per OPC-10000-4 section 7.22.4 in tests/unit/test_aggregate.c
- [X] T060 [US3] Add the QueryNext invalid continuation point Bad_ContinuationPointInvalid test per OPC-10000-4 sections B.2.4 and 7.9 in tests/unit/test_query_service.c
- [X] T061a [US3] Add the NodeManagement BrowseName lifetime stability test per OPC-10000-4 section 5.8.2 in tests/unit/test_node_management_errors.c
- [X] T061b [US3] Add the NodeManagement DisplayName lifetime stability test per OPC-10000-4 section 5.8.2 in tests/unit/test_node_management_errors.c
- [X] T061c [US3] Add the NodeManagement string NodeId lifetime stability test per OPC-10000-4 section 5.8.2 in tests/unit/test_node_management_errors.c
- [X] T061d [US3] Add the NodeManagement reference lifetime stability test per OPC-10000-4 section 5.8.3 in tests/unit/test_node_management_errors.c
- [X] T062a [US3] Add the HistoryRead continuation-point ownership test per OPC-10000-4 sections 5.11.3.2 and 7.9 in tests/unit/test_history.c
- [X] T062b [US3] Add the HistoryRead later-request stability test per OPC-10000-4 sections 5.11.3.2 and 7.9 in tests/unit/test_history.c
- [X] T063 [US3] Add the Publish oversize response Bad_ResponseTooLarge test per OPC-10000-4 sections 5.3, 5.14.5.1, and 7.38.2 in tests/unit/test_subscriptions_errors.c
- [X] T064 [US3] Add the unsupported service dispatch Bad_ServiceUnsupported test per OPC-10000-4 sections 6.3.1 and 7.38.2 in tests/unit/test_dispatch_services.c

### Implementation for User Story 3

- [X] T065a [US3] [Resource: Throughput] Implement FindServers endpoint URL filtering per OPC-10000-4 section 5.5.2.2 in src/services/discovery.c
- [X] T065b [US3] [Resource: Throughput] Implement FindServers locale filtering per OPC-10000-4 section 5.5.2.2 in src/services/discovery.c
- [X] T065c [US3] [Resource: Throughput] Implement FindServers server URI filtering per OPC-10000-4 section 5.5.2.2 in src/services/discovery.c
- [X] T065d [US3] [Resource: Throughput] Implement FindServers server type filtering per OPC-10000-4 section 5.5.2.2 in src/services/discovery.c
- [X] T066a [US3] [Resource: Throughput] Implement GetEndpoints endpoint URL filtering per OPC-10000-4 section 5.5.4.2 in src/services/discovery.c
- [X] T066b [US3] [Resource: Throughput] Implement GetEndpoints locale filtering per OPC-10000-4 section 5.5.4.2 in src/services/discovery.c
- [X] T066d [US3] [Resource: Throughput] Implement GetEndpoints transport profile filtering per OPC-10000-4 section 5.5.4.2 in src/services/discovery.c
- [X] T067a [US3] [Resource: Flash/Throughput] Implement CreateMonitoredItems unsupported MonitoringFilter mapping to Bad_MonitoredItemFilterUnsupported per OPC-10000-4 section 5.13.2.4 in src/services/subscription.c
- [X] T067b [US3] [Resource: Flash/Throughput] Implement CreateMonitoredItems invalid MonitoringFilter mapping to Bad_MonitoredItemFilterInvalid per OPC-10000-4 section 5.13.2.4 in src/services/subscription.c
- [X] T067c [US3] [Resource: Flash/Throughput] Implement CreateMonitoredItems disallowed MonitoringFilter mapping to Bad_FilterNotAllowed per OPC-10000-4 section 5.13.2.4 in src/services/subscription.c
- [X] T067d [US3] [Resource: Flash/Throughput] Implement ModifyMonitoredItems unsupported MonitoringFilter mapping to Bad_MonitoredItemFilterUnsupported per OPC-10000-4 section 5.13.3.4 in src/services/subscription.c
- [X] T067e [US3] [Resource: Flash/Throughput] Implement ModifyMonitoredItems invalid MonitoringFilter mapping to Bad_MonitoredItemFilterInvalid per OPC-10000-4 section 5.13.3.4 in src/services/subscription.c
- [X] T067f [US3] [Resource: Flash/Throughput] Implement ModifyMonitoredItems disallowed MonitoringFilter mapping to Bad_FilterNotAllowed per OPC-10000-4 section 5.13.3.4 in src/services/subscription.c
- [X] T068a [US3] [Resource: Flash/RAM/Throughput] Implement AggregateFilter Average scoped support per OPC-10000-4 section 7.22.4 and OPC-10000-13 section 5.4.3.5 in src/services/subscription.c
- [X] T068b [US3] [Resource: Flash/RAM/Throughput] Implement AggregateFilter Minimum scoped support per OPC-10000-4 section 7.22.4 and OPC-10000-13 section 5.4.3.10 in src/services/subscription.c
- [X] T068c [US3] [Resource: Flash/RAM/Throughput] Implement AggregateFilter Maximum scoped support per OPC-10000-4 section 7.22.4 and OPC-10000-13 section 5.4.3.11 in src/services/subscription.c
- [X] T068d [US3] [Resource: Flash/Throughput] Implement unsupported AggregateFilter rejection returning Bad_MonitoredItemFilterUnsupported per OPC-10000-4 section 7.22.4 in src/services/subscription.c
- [X] T069 [US3] [Resource: RAM/Throughput] Implement QueryNext invalid continuation point rejection returning Bad_ContinuationPointInvalid per OPC-10000-4 sections B.2.4 and 7.9 in src/services/query.c
- [X] T070a [US3] [Resource: RAM/Flash] Implement bounded owned NodeManagement BrowseName storage per OPC-10000-4 section 5.8.2 in src/services/node_management.c
- [X] T070b [US3] [Resource: RAM/Flash] Implement bounded owned NodeManagement DisplayName storage per OPC-10000-4 section 5.8.2 in src/services/node_management.c
- [X] T070c [US3] [Resource: RAM/Flash] Implement bounded owned NodeManagement string NodeId storage per OPC-10000-4 section 5.8.2 in src/services/node_management.c
- [X] T070d [US3] [Resource: RAM/Flash] Implement bounded owned NodeManagement reference storage per OPC-10000-4 section 5.8.3 in src/services/node_management.c
- [X] T071 [US3] [Resource: RAM/Flash] Implement bounded owned HistoryRead continuation-point storage per OPC-10000-4 sections 5.11.3.2 and 7.9 in src/services/history.c
- [X] T072 [US3] [Resource: RAM/Throughput] Implement Publish oversize response handling returning Bad_ResponseTooLarge per OPC-10000-4 sections 5.3, 5.14.5.1, and 7.38.2 in src/services/subscription.c
- [X] T073 [US3] [Resource: Flash/Throughput] Implement unsupported service dispatch returning Bad_ServiceUnsupported per OPC-10000-4 sections 6.3.1 and 7.38.2 in src/core/service_dispatch.c
- [X] T074a [US3] Run the US3 Discovery filter tests referenced by docs/validation/audit-hardening.md
- [X] T074b [US3] Record the US3 Discovery filter evidence in docs/validation/audit-hardening.md
- [X] T074c [US3] Run the US3 MonitoringFilter tests referenced by docs/validation/audit-hardening.md
- [X] T074d [US3] Record the US3 MonitoringFilter evidence in docs/validation/audit-hardening.md
- [X] T074e [US3] Run the US3 AggregateFilter tests referenced by docs/validation/audit-hardening.md
- [X] T074f [US3] Record the US3 AggregateFilter evidence in docs/validation/audit-hardening.md
- [X] T074g [US3] Run the US3 Query tests referenced by docs/validation/audit-hardening.md
- [X] T074h [US3] Record the US3 Query evidence in docs/validation/audit-hardening.md
- [X] T074i [US3] Run the US3 History tests referenced by docs/validation/audit-hardening.md
- [X] T074j [US3] Record the US3 History evidence in docs/validation/audit-hardening.md
- [X] T074k [US3] Run the US3 NodeManagement tests referenced by docs/validation/audit-hardening.md
- [X] T074l [US3] Record the US3 NodeManagement evidence in docs/validation/audit-hardening.md
- [X] T074m [US3] Run the US3 Publish tests referenced by docs/validation/audit-hardening.md
- [X] T074n [US3] Record the US3 Publish evidence in docs/validation/audit-hardening.md
- [X] T074o [US3] Run the US3 dispatch tests referenced by docs/validation/audit-hardening.md
- [X] T074p [US3] Record the US3 dispatch evidence in docs/validation/audit-hardening.md

**Checkpoint**: US3 is independently testable and all claimed service semantics are either implemented or rejected with cited StatusCodes.

---

## Phase 6: User Story 4 - Keep Conformance Claims and Verification Honest (Priority: P3)

**Goal**: Documentation, traceability, verification gates, size evidence, stack evidence, and closure records accurately match implemented behavior and do not overclaim profile compliance.

**Independent Test**: Run conformance-doc tests, validation scripts, size and stack measurement, and closure-matrix review; verify no unsupported profile-compliant or CTT-verified claims remain.

### Tests for User Story 4

- [X] T075a [US4] Add the profile-compliant claim rejection test per OPC-10000-7 sections 4.2 and 4.3 in tests/unit/test_conformance_docs.c
- [X] T075b [US4] Add the CTT-verified claim rejection test per OPC-10000-7 sections 4.2 and 4.3 in tests/unit/test_conformance_docs.c
- [X] T076a [US4] Add the StatusCode name documentation consistency test per OPC-10000-4 section 7.38.2 in tests/unit/test_conformance_docs.c
- [X] T076b [US4] Add the StatusCode numeric documentation consistency test per OPC-10000-4 section 7.38.2 in tests/unit/test_conformance_docs.c
- [X] T077a [US4] Add the files-to-sections traceability test for changed protocol files in tests/unit/test_traceability_docs.c
- [X] T077b [US4] Add the sections-to-files traceability test for cited OPC UA sections in tests/unit/test_traceability_docs.c
- [X] T078a [US4] Add the fuzz TODO placeholder rejection test in tests/unit/test_conformance_docs.c
- [X] T078b [US4] Add the fuzz unused-data placeholder rejection test in tests/unit/test_conformance_docs.c

### Implementation for User Story 4

- [X] T079a [US4] Update service claims to profile-targeting with OPC-10000-7 sections 4.2 and 4.3 citations in docs/conformance/services.md
- [X] T079b [US4] Update profile claim transport and encoding details with OPC-10000-7 sections 4.2 and 4.3 and OPC-10000-6 sections 5.2, 7.1.2.2, 7.1.2.3, 7.1.2.4, and 7.2 citations in docs/conformance/profile-embedded.md
- [X] T080a [US4] Update SecurityPolicy None documentation with OPC-10000-4 section 5.5.4.2 citations in docs/conformance/security.md
- [X] T080b [US4] Update username/password token policy documentation with OPC-10000-4 section 5.7.3 citations in docs/conformance/identity-policy.md
- [X] T081a [US4] Update exact StatusCode names after T007a through T009r with OPC-10000-4 section 7.38.2 citations in docs/conformance/status.md
- [X] T081b [US4] Update exact StatusCode numeric values after T007a through T009r with OPC-10000-4 section 7.38.2 citations in docs/conformance/status.md
- [X] T082a [US4] Update file-to-section traceability for changed source files in docs/traceability/files-to-sections.md
- [X] T082b [US4] Update file-to-section traceability for changed test files in docs/traceability/files-to-sections.md
- [X] T082c [US4] Update file-to-section traceability for changed documentation files in docs/traceability/files-to-sections.md
- [X] T083 [US4] Update section-to-file traceability for cited OPC UA sections in docs/traceability/sections-to-files.md
- [X] T084a [US4] Update AggregateFilter Average scoped-support documentation with OPC-10000-4 section 7.22.4 and OPC-10000-13 sections 4.2.2.4 and 5.4.3.5 citations in docs/conformance/services.md
- [X] T084b [US4] Update AggregateFilter Minimum scoped-support documentation with OPC-10000-4 section 7.22.4 and OPC-10000-13 sections 4.2.2.9 and 5.4.3.10 citations in docs/conformance/services.md
- [X] T084c [US4] Update AggregateFilter Maximum scoped-support documentation with OPC-10000-4 section 7.22.4 and OPC-10000-13 sections 4.2.2.10 and 5.4.3.11 citations in docs/conformance/services.md
- [X] T084d [US4] Update unsupported AggregateFilter documentation with OPC-10000-4 section 7.22.4 and OPC-10000-13 sections 4.2.2.4, 4.2.2.9, 4.2.2.10, 5.4.3.5, 5.4.3.10, and 5.4.3.11 citations in docs/conformance/services.md
- [X] T085a [US4] Update host-test required-gate status in docs/validation/host-tests.md
- [X] T085b [US4] Update host-test evidence for this feature in docs/validation/host-tests.md
- [X] T086a [US4] Update sanitizer required-gate status in docs/validation/sanitizers.md
- [X] T086b [US4] Update sanitizer evidence for this feature in docs/validation/sanitizers.md
- [X] T087a [US4] Update binary reader fuzz evidence in docs/validation/fuzz.md
- [X] T087b [US4] Update message chunk fuzz evidence in docs/validation/fuzz.md
- [X] T087c [US4] Update ActivateSession fuzz evidence in docs/validation/fuzz.md
- [X] T087d [US4] Update binary types fuzz evidence in docs/validation/fuzz.md
- [X] T087e [US4] Update subscription filters fuzz evidence in docs/validation/fuzz.md
- [X] T088a [US4] [Resource: Flash/RAM] Update host flash and RAM evidence in docs/size/feature-size-ledger.md
- [X] T088b [US4] [Resource: Flash/RAM] Update embedded flash and RAM evidence in docs/size/feature-size-ledger.md
- [X] T088c [US4] [Resource: RAM] Update data and bss evidence in docs/size/feature-size-ledger.md
- [X] T088d [US4] [Resource: Stack] Update stack evidence in docs/size/feature-size-ledger.md
- [X] T088e [US4] [Resource: Flash/RAM/Stack] Update application-headroom evidence against the +8 KiB host/full text, +4 KiB nano/embedded text, no-new-default-static-RAM, no-hot-path-heap, and stack-recording budget in docs/size/feature-size-ledger.md
- [X] T089a [US4] [Resource: Flash/RAM] Update Pico minimal server size evidence in docs/size/pico-minimal-server.md
- [X] T089b [US4] [Resource: Stack] Update Pico minimal server stack evidence in docs/size/pico-minimal-server.md
- [X] T090a [US4] Update no-heap evidence in docs/review/embedded-readiness-review.md
- [X] T090b [US4] Update transport-buffer evidence in docs/review/embedded-readiness-review.md
- [X] T090c [US4] Update static-storage evidence in docs/review/embedded-readiness-review.md
- [X] T090d [US4] Update application-headroom review evidence against the +8 KiB host/full text, +4 KiB nano/embedded text, no-new-default-static-RAM, no-hot-path-heap, and stack-recording budget in docs/review/embedded-readiness-review.md
- [X] T091a [US4] Mark fixed audit findings in docs/validation/audit-hardening-closure.md
- [X] T091b [US4] Mark profile-disabled audit findings in docs/validation/audit-hardening-closure.md
- [X] T091c [US4] Mark explicitly deferred audit findings with risk rationale in docs/validation/audit-hardening-closure.md
- [X] T092a [US4] Update the host command transcript in docs/validation/audit-hardening.md
- [X] T092b [US4] Update the sanitizer command transcript in docs/validation/audit-hardening.md
- [X] T092c [US4] Update the fuzz command transcript in docs/validation/audit-hardening.md
- [X] T092d [US4] Update the embedded command transcript in docs/validation/audit-hardening.md
- [X] T092e [US4] Update the conformance-doc command transcript in docs/validation/audit-hardening.md
- [X] T092f [US4] Update the size command transcript in docs/validation/audit-hardening.md
- [X] T092g [US4] Update the formatting/static-analysis command transcript in docs/validation/audit-hardening.md
- [X] T092h [US4] Update the latency-threshold command transcript in docs/validation/audit-hardening.md

**Checkpoint**: US4 is independently testable and prevents green validation from hiding undocumented conformance or resource regressions.

---

## Phase 7: Compliance, Size, and Polish

**Purpose**: Cross-cutting verification required before implementation is complete.

- [X] T093a Run the host build command documented in docs/validation/audit-hardening.md
- [X] T093b Record the host build result in docs/validation/audit-hardening.md
- [X] T094a Run the host CTest command documented in docs/validation/audit-hardening.md
- [X] T094b Record the host CTest result in docs/validation/audit-hardening.md
- [X] T095a Run the sanitizer build command documented in docs/validation/audit-hardening.md
- [X] T095b Record the sanitizer build result in docs/validation/audit-hardening.md
- [X] T096a Run the sanitizer CTest command documented in docs/validation/audit-hardening.md
- [X] T096b Record the sanitizer CTest result in docs/validation/audit-hardening.md
- [X] T097a Run the binary reader fuzz smoke command documented in docs/validation/fuzz.md
- [X] T097b Record the binary reader fuzz result in docs/validation/fuzz.md
- [X] T097c Run the message chunk fuzz smoke command documented in docs/validation/fuzz.md
- [X] T097d Record the message chunk fuzz result in docs/validation/fuzz.md
- [X] T097e Run the ActivateSession fuzz smoke command documented in docs/validation/fuzz.md
- [X] T097f Record the ActivateSession fuzz result in docs/validation/fuzz.md
- [X] T097g Run the binary types fuzz smoke command documented in docs/validation/fuzz.md
- [X] T097h Record the binary types fuzz result in docs/validation/fuzz.md
- [X] T097i Run the subscription filters fuzz smoke command documented in docs/validation/fuzz.md
- [X] T097j Record the subscription filters fuzz result in docs/validation/fuzz.md
- [X] T098a Run the build-matrix command documented in docs/validation/audit-hardening.md
- [X] T098b Record the build-matrix result in docs/validation/audit-hardening.md
- [X] T098c Run the formatting/static-analysis command documented in docs/validation/static-analysis.md
- [X] T098d Record the formatting/static-analysis result in docs/validation/static-analysis.md
- [X] T098e [Resource: Throughput] Run the valid minimal discovery/session/read latency comparison command documented in docs/validation/audit-hardening.md
- [X] T098f [Resource: Throughput] Record the valid minimal discovery/session/read latency comparison against the 5% threshold in docs/validation/audit-hardening.md
- [X] T099a [Resource: Flash/RAM] Run the size-report command documented in docs/size/feature-size-ledger.md
- [X] T099b [Resource: Flash/RAM] Record the raw size-report output in docs/size/feature-size-ledger.md
- [X] T099c [Resource: Flash/RAM] Compare the size-report output against the plan budget in docs/size/feature-size-ledger.md
- [X] T100a [Resource: Stack] Run the stack-usage command documented in docs/size/feature-size-ledger.md
- [X] T100b [Resource: Stack] Record the stack-usage result in docs/size/feature-size-ledger.md
- [X] T101a Run the profile-compliance claim grep documented in docs/validation/audit-hardening.md
- [X] T101b Record the profile-compliance claim grep result in docs/validation/audit-hardening.md
- [X] T101c Run the placeholder traceability grep documented in docs/validation/audit-hardening.md
- [X] T101d Record the placeholder traceability grep result in docs/validation/audit-hardening.md
- [X] T101e Run the StatusCode and OPC citation grep documented in docs/validation/audit-hardening.md
- [X] T101f Record the StatusCode and OPC citation grep result in docs/validation/audit-hardening.md
- [X] T102a Verify P0, P1, and High audit finding closure status in docs/validation/audit-hardening-closure.md
- [X] T102b Record the audit finding closure verification in docs/validation/audit-hardening-closure.md

---

## Dependencies & Execution Order

### Phase Dependencies

- **Setup (Phase 1)**: No dependencies.
- **Foundational (Phase 2)**: Depends on Setup and blocks all user stories.
- **US1 (Phase 3)**: Depends on Foundation; MVP scope.
- **US2 (Phase 4)**: Depends on Foundation and can run after US1 tests are in place if file conflicts are managed manually.
- **US3 (Phase 5)**: Depends on Foundation and benefits from US1 decoder/status behavior.
- **US4 (Phase 6)**: Depends on completed behavior from US1 through US3 for final claims, but its test scaffolding can begin after Foundation.
- **Compliance/Polish (Phase 7)**: Depends on selected user stories being complete.

### Within Each User Story

- Add RED tests before implementation.
- Keep each implementation task scoped to the cited file and StatusCode behavior.
- Update traceability and resource evidence before checkpoint completion.
- Re-run the story-specific tests before moving to the next story.

### User Story Dependency Graph

```text
Phase 1 Setup
  -> Phase 2 Foundation
      -> US1 Reject malformed traffic [MVP]
      -> US2 Preserve security
      -> US3 Match claimed service semantics
          -> US4 Honest conformance and verification evidence
              -> Phase 7 Compliance, size, and polish
```

### Parallel Opportunities

No tasks are marked `[P]`. This is intentional for this remediation pass because the atomic task list contains many same-file edits in adjacent tasks. Parallel execution should be decided manually from the write set after an implementer claims a contiguous task range.

---

## Implementation Strategy

### MVP First

Complete Phase 1, Phase 2, and US1 first. This closes the most dangerous malformed-input class and proves exact StatusCodes, decoder boundaries, service body completeness, and no state mutation before broader service semantics.

### Incremental Delivery

1. Finish shared status, decoder, transport, and dispatch foundations.
2. Complete US1 malformed-input behavior and record evidence.
3. Complete US2 security behavior and record evidence.
4. Complete US3 claimed service semantics and optional-profile storage behavior.
5. Complete US4 documentation, verification, and resource evidence.
6. Run Phase 7 release validation.

### Resource-Risk Handling

Tasks marked with `[Resource: Flash]`, `[Resource: RAM]`, `[Resource: Stack]`, or `[Resource: Throughput]` require extra review before merge. For each marked production task, compare the final evidence in docs/size/feature-size-ledger.md against the plan budget and confirm the change does not add heap use to the protocol hot path.
