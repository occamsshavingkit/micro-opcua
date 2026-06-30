# Tasks: OPC UA PubSub (UADP/UDP)

**Input**: Design documents from `/specs/012-opcua-pubsub/`
**Prerequisites**: plan.md (required), spec.md (required for user stories), research.md, data-model.md, quickstart.md

## Phase 1: Setup (Shared Infrastructure)

**Purpose**: Project initialization and traceability structure

- [X] T001 Update `CMakeLists.txt` to add `MICRO_OPCUA_PUBSUB` configuration toggle and conditional source inclusions for `pubsub.c` and `uadp_encoder.c`.
- [X] T002 Add traceability table skeleton in `docs/traceability/012-opcua-pubsub.md`.

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Core infrastructure that MUST be complete before user story work

- [X] T003 Document target OPC UA profile (PubSub UADP UDP Publisher) from OPC-10000-14 in `docs/traceability/012-opcua-pubsub.md`.
- [X] T004 [P] Define public configuration API in `include/micro_opcua/pubsub.h` containing `mu_pubsub_connection_t`, `mu_pubsub_writer_group_t`, `mu_pubsub_dataset_writer_t`, and `mu_pubsub_field_t`.
- [X] T005 [P] Add PubSub configuration structure to `mu_server_config_t` in `include/micro_opcua/server.h`.
- [X] T006 [P] Define UDP platform adapter interface in `include/micro_opcua/platform.h` for UDP payload broadcasting.

**Checkpoint**: Foundation ready - user story implementation can begin.

---

## Phase 3: User Story 1 - UDP Broadcasting (Priority: P1) [MVP]

**Goal**: A sensor device needs to broadcast its current temperature every 1 second over UDP without accepting incoming TCP connections to preserve memory and power.

**Independent Test**: Can be tested by running the host server, capturing UDP packets on localhost using Wireshark, and decoding them as OPC UA UADP.

### Tests for User Story 1

> Write these tests first and confirm they fail before implementation.

- [X] T007 [P] [US1] Add byte-level fixture assertions for a scoped UADP NetworkMessage citing OPC-10000-14 §7.2.4.4.2, §7.2.4.5.2, §7.2.4.5.4, and §7.2.4.5.5 in `tests/unit/test_uadp_encoding.c`.
- [X] T008 [P] [US1] Add round-trip unit test for UADP binary encoder in `tests/unit/test_uadp_encoding.c`.
- [X] T009 [P] [US1] Add unit tests for Publisher state machine and timing logic in `tests/unit/test_pubsub.c`.

### Implementation for User Story 1

- [X] T010 [P] [US1] Implement UADP binary encoder `mu_encode_uadp_network_message` in `src/encoding/uadp_encoder.c`.
- [X] T011 [US1] Implement PubSub publisher tick logic in `src/core/pubsub.c` that iterates over writer groups, checks timing, and formats UDP payloads.
- [X] T012 [US1] Update `mu_server_poll` in `src/core/server.c` to call the PubSub tick when `MICRO_OPCUA_PUBSUB` is ON.
- [X] T013 [US1] Implement host POSIX UDP adapter in `src/platform/host_udp_adapter.c`.
- [X] T014 [US1] Create example `examples/pubsub_server/main.c` demonstrating connectionless UDP broadcasting.
- [X] T015 [US1] Update `docs/traceability/012-opcua-pubsub.md` with implementation and test file mappings to OPC-10000-14.

**Checkpoint**: User Story 1 is independently testable and broadcasts UADP packets.

---

## Phase 4: Compliance, Size, and Polish

**Purpose**: Cross-cutting validation required before completion

- [X] T016 Run host unit and integration tests (`cmake --build build/pubsub-hardening -j$(nproc)` and `ctest --test-dir build/pubsub-hardening --output-on-failure`).
- [X] T017 Run formatting and static analysis (`format-check` and `cppcheck`).
- [X] T018 Run embedded cross-compile for Raspberry Pi Pico (`build/pubsub-pico` with `MICRO_OPCUA_PLATFORM=pico` and examples enabled).
- [X] T019 Measure flash/RAM impact of `pubsub.o` and `uadp_encoder.o` and ensure they fit the < 3KB flash and < 200 byte RAM budget.
- [X] T020 Validate `quickstart.md` code against `examples/pubsub_server/main.c`.

---

## Dependencies & Execution Order

### Phase Dependencies
- **Setup (Phase 1)**: No dependencies
- **Foundational (Phase 2)**: Depends on Setup
- **User Stories (Phase 3+)**: Depend on Foundational
- **Compliance/Polish (Phase 4)**: Depends on Phase 3

### Within User Story 1
- UADP fixtures and encoding tests must be written before the encoder.
- The encoder must be complete before the `pubsub.c` tick logic.
- The platform adapter is required for the example server.
