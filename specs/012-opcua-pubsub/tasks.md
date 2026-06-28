# Tasks: OPC UA PubSub (UADP/UDP)

**Input**: Design documents from `/specs/012-opcua-pubsub/`
**Prerequisites**: plan.md (required), spec.md (required for user stories), research.md, data-model.md, quickstart.md

## Phase 1: Setup (Shared Infrastructure)

**Purpose**: Project initialization and traceability structure

- [ ] T001 Update `CMakeLists.txt` to add `MICRO_OPCUA_PUBSUB` configuration toggle and conditional source inclusions for `pubsub.c` and `uadp_encoder.c`.
- [ ] T002 Add traceability table skeleton in `docs/traceability/012-opcua-pubsub.md`.

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Core infrastructure that MUST be complete before user story work

- [ ] T003 Document target OPC UA profile (PubSub UADP UDP Publisher) from OPC-10000-14 in `docs/traceability/012-opcua-pubsub.md`.
- [ ] T004 [P] Define public configuration API in `include/micro_opcua/pubsub.h` containing `mu_pubsub_connection_t`, `mu_pubsub_writer_group_t`, and `mu_pubsub_dataset_writer_t`.
- [ ] T005 [P] Add PubSub configuration structure to `mu_server_config_t` in `include/micro_opcua/config.h`.
- [ ] T006 [P] Define `mu_udp_send` platform adapter interface in `include/micro_opcua/platform.h` for UDP payload broadcasting.

**Checkpoint**: Foundation ready - user story implementation can begin.

---

## Phase 3: User Story 1 - UDP Broadcasting (Priority: P1) [MVP]

**Goal**: A sensor device needs to broadcast its current temperature every 1 second over UDP without accepting incoming TCP connections to preserve memory and power.

**Independent Test**: Can be tested by running the host server, capturing UDP packets on localhost using Wireshark, and decoding them as OPC UA UADP.

### Tests for User Story 1

> Write these tests first and confirm they fail before implementation.

- [ ] T007 [P] [US1] Add binary fixture for UADP NetworkMessage citing OPC-10000-14 7.2 in `tests/fixtures/uadp_network_message.bin`.
- [ ] T008 [P] [US1] Add round-trip unit test for UADP binary encoder in `tests/unit/test_uadp_encoding.c`.
- [ ] T009 [P] [US1] Add unit tests for Publisher state machine and timing logic in `tests/unit/test_pubsub.c`.

### Implementation for User Story 1

- [ ] T010 [P] [US1] Implement UADP binary encoder `mu_encode_uadp_network_message` in `src/encoding/uadp_encoder.c`.
- [ ] T011 [US1] Implement PubSub publisher tick logic in `src/core/pubsub.c` that iterates over writer groups, checks timing, and formats UDP payloads.
- [ ] T012 [US1] Update `mu_server_poll` in `src/core/server.c` to call the PubSub tick when `MICRO_OPCUA_PUBSUB` is ON.
- [ ] T013 [US1] Implement host POSIX UDP adapter `mu_udp_send` in `platform/host/udp_adapter.c`.
- [ ] T014 [US1] Create example `examples/pubsub_server/main.c` demonstrating connectionless UDP broadcasting.
- [ ] T015 [US1] Update `docs/traceability/012-opcua-pubsub.md` with implementation and test file mappings to OPC-10000-14.

**Checkpoint**: User Story 1 is independently testable and broadcasts UADP packets.

---

## Phase 4: Compliance, Size, and Polish

**Purpose**: Cross-cutting validation required before completion

- [ ] T016 Run host unit and integration tests (`make test`).
- [ ] T017 Run formatting and static analysis (`make check` or similar).
- [ ] T018 Run embedded cross-compile for Raspberry Pi Pico (`make pico`).
- [ ] T019 Measure flash/RAM impact of `pubsub.o` and `uadp_encoder.o` and ensure they fit the < 3KB flash and < 200 byte RAM budget.
- [ ] T020 Validate `quickstart.md` code against `examples/pubsub_server/main.c`.

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
