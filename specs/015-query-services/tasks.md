# Tasks: Query Services

**Input**: Design documents from `specs/015-query-services/`
**Prerequisites**: plan.md, spec.md

**Tests**: Tests are mandatory for protocol parsing, serialization, service dispatch, StatusCodes, SecureChannel/session state, address-space behavior, security behavior, and wire-visible changes.

## Phase 1: Setup & Definition

**Purpose**: Core definitions for the Query services and ContentFilter parsing.

- [x] `T001` [US1] Define Query service structures: `mu_query_first_request_t`, `mu_query_first_response_t`, `mu_query_next_request_t`, `mu_query_next_response_t` in `src/services/query.h`. Include OPC-10000-4 Appendix B.2.3 and B.2.4 references.
- [x] `T002` [US1] Define `ContentFilter` and `ContentFilterElement` structures in `src/services/query.h`. Include subset of standard operators per OPC-10000-4 7.7.
- [x] `T003` [US1] Register `QueryFirstRequest_Encoding_DefaultBinary` (615), `QueryFirstResponse` (618), `QueryNextRequest` (621), `QueryNextResponse` (624) IDs in `include/micro_opcua/opcua_ids.h`.
- [x] `T004` [US1] Add `query_context` to `mu_server_t` in `src/core/server_internal.h` and initialize it in `src/core/server.c` (`mu_server_init`).

## Phase 2: Encoding & Decoding (US1 & US2)

**Purpose**: Binary parsing for queries.

- [x] `T005` [P] [US1] Implement `mu_content_filter_decode` in `src/encoding/binary_query.c` to parse a `ContentFilter` (array of `ContentFilterElement`).
- [x] `T006` [US1] Implement `mu_query_first_request_decode` in `src/encoding/binary_query.c`.
- [x] `T007` [US1] Implement `mu_query_first_response_encode` in `src/encoding/binary_query.c`.
- [x] `T008` [US2] Implement `mu_query_next_request_decode` and `mu_query_next_response_encode` in `src/encoding/binary_query.c`.
- [x] `T009` [US1] Write unit tests in `tests/unit/test_query_encoding.c` to verify encoding/decoding of QueryFirst and ContentFilters.

## Phase 3: Service Logic (US1 & US2)

**Purpose**: Implementation of the search capability.

- [x] `T010` [US1] Implement scoped `mu_evaluate_content_filter` in `src/services/query.c` for the currently supported subset.
- [x] `T011` [US1] Implement `mu_query_first_process` in `src/services/query.c` to scan the static/dynamic address space, apply the filter, and populate the response, generating a `ContinuationPoint` if `MaxQueryResults` is hit.
- [x] `T012` [US2] Implement `mu_query_next_process` in `src/services/query.c` to validate/release continuation points.
- [x] `T013` [US1] Wire up `mu_query_first_process` and `mu_query_next_process` in `src/core/service_dispatch.c` (`handle_query_first` and `handle_query_next`).
- [x] `T014` [US1] Add `QueryFirst` / `QueryNext` to the test coverage suite. Create `tests/unit/test_query_service.c` validating scoped processing and `ContinuationPoint` behavior.

## Phase 4: Integration, Limits, and Docs

**Purpose**: Final compliance, testing, and memory validation.

- [x] `T015` [US1] Add `MU_MAX_QUERY_CONTINUATION_POINTS` (default 2) to `include/micro_opcua/config.h` and increase `MU_SERVER_STORAGE_BYTES` if query services are enabled via `-DMICRO_OPCUA_SERVICE_QUERY=1`.
- [ ] `T016` [US1] Run `measure_size.sh` to capture the flash/RAM impact.
- [x] `T017` [US1] Add `binary_query.c`, `query.c`, and `test_query_service.c` to traceability matrix `docs/traceability/files-to-sections.md`.
- [ ] `T018` [US1] Run formatting, cppcheck, and ensure all tests pass (CTEST).
