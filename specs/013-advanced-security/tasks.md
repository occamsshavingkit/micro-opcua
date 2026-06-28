# Tasks: Advanced Security Policies

**Input**: Design documents from `/specs/013-advanced-security/`
**Prerequisites**: plan.md (required), spec.md (required for user stories),
research.md, data-model.md, quickstart.md

## Phase 1: Setup (Shared Infrastructure)

**Purpose**: Project initialization, build tooling, and traceability structure

- [ ] T001 Define `mu_security_policy_t` values for `Aes128-Sha256-RsaOaep` and `Aes256-Sha256-RsaPss` in `include/micro_opcua/security.h`

---

## Phase 2: Foundational (Blocking Prerequisites)

**Purpose**: Core infrastructure that MUST be complete before user story work

**CRITICAL**: No user story implementation can begin until this phase is complete.

- [ ] T002 Define `mu_trust_list_t` struct in `include/micro_opcua/security.h`
- [ ] T003 Update `mu_server_config_t` to include `mu_trust_list_t*` in `include/micro_opcua/server.h`
- [ ] T004 [P] Update `mu_crypto_adapter_t` to support generic asymmetric functions taking security policy as an argument (or separate RSA-OAEP / RSA-PSS functions) in `include/micro_opcua/crypto_adapter.h`
- [ ] T005 [P] Create mock crypto adapters in tests to simulate successful/failed verifications and decryptions in `tests/unit/mock_crypto_adapter.h`

**Checkpoint**: Foundation ready - user story implementation can begin.

---

## Phase 3: User Story 1 - Secure Channel with Aes128-Sha256-RsaOaep (Priority: P1) [MVP]

**Goal**: Support the `Aes128-Sha256-RsaOaep` security policy.

**Independent Test**: Connect external client with `Aes128-Sha256-RsaOaep` only and ensure telemetry can be read.

### Tests for User Story 1

- [ ] T006 [P] [US1] Add round-trip unit test for RSA-OAEP encrypt/decrypt in `tests/unit/test_security_rsa_oaep.c`
- [ ] T007 [P] [US1] Add malformed-input unit test for RSA-OAEP decryption in `tests/unit/test_security_rsa_oaep_errors.c`

### Implementation for User Story 1

- [ ] T008 [P] [US1] Implement RSA-OAEP in `src/platform/host_crypto_adapter.c` (OpenSSL backend)
- [ ] T009 [P] [US1] Implement RSA-OAEP in `src/platform/mbedtls_crypto_adapter.c` (mbedTLS backend)
- [ ] T010 [US1] Update `src/core/service_dispatch.c` to dispatch `Aes128-Sha256-RsaOaep` correctly during `OpenSecureChannel`

**Checkpoint**: User Story 1 is independently testable and size impact is known.

---

## Phase 4: User Story 2 - Secure Channel with Aes256-Sha256-RsaPss (Priority: P1)

**Goal**: Support the `Aes256-Sha256-RsaPss` security policy.

**Independent Test**: Connect external client with `Aes256-Sha256-RsaPss` only and ensure secure channel establishes.

### Tests for User Story 2

- [ ] T011 [P] [US2] Add round-trip unit test for RSA-PSS sign/verify in `tests/unit/test_security_rsa_pss.c`
- [ ] T012 [P] [US2] Add error unit test for invalid RSA-PSS signature in `tests/unit/test_security_rsa_pss_errors.c`

### Implementation for User Story 2

- [ ] T013 [P] [US2] Implement RSA-PSS in `src/platform/host_crypto_adapter.c` (OpenSSL backend)
- [ ] T014 [P] [US2] Implement RSA-PSS in `src/platform/mbedtls_crypto_adapter.c` (mbedTLS backend)
- [ ] T015 [US2] Update `src/core/service_dispatch.c` to dispatch `Aes256-Sha256-RsaPss` correctly during `OpenSecureChannel`

**Checkpoint**: User Stories 1 and 2 both work independently.

---

## Phase 5: User Story 3 - Client Certificate TrustList Management (Priority: P2)

**Goal**: Enforce client certificate validation against a TrustList during `OpenSecureChannel`.

**Independent Test**: Attempt connection with untrusted certificate -> reject. Attempt with trusted certificate -> accept.

### Tests for User Story 3

- [ ] T016 [P] [US3] Add unit test for exact matching of client certificate against TrustList in `tests/unit/test_trustlist.c`
- [ ] T017 [P] [US3] Add unit test for empty/null TrustList rejecting all secure connections in `tests/unit/test_trustlist_errors.c`

### Implementation for User Story 3

- [ ] T018 [P] [US3] Implement TrustList matching function `mu_trust_list_match` in `src/security/trustlist.c` (new file)
- [ ] T019 [US3] Integrate `mu_trust_list_match` in `src/core/service_dispatch.c` during `OpenSecureChannel` handling, returning `Bad_SecurityChecksFailed` on failure.

**Checkpoint**: Selected user stories are independently functional.

---

## Phase N: Compliance, Size, and Polish

**Purpose**: Cross-cutting validation required before completion

- [ ] T020 Run host unit and integration tests
- [ ] T021 Run formatting and static analysis
- [ ] T022 Run embedded cross-compile using Pico SDK and Arduino environments
- [ ] T023 Measure flash/RAM impact and record in `plan.md`
- [ ] T024 Validate quickstart example code compiles in `examples/minimal_server/`

---

## Dependencies & Execution Order

### Phase Dependencies

- **Setup (Phase 1)**: No dependencies
- **Foundational (Phase 2)**: Depends on Setup completion; blocks all stories
- **User Stories (Phase 3+)**: Depend on Foundational phase completion
- **Compliance/Polish**: Depends on all desired user stories being complete
