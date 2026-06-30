# Implementation Plan: Audit Hardening

**Branch**: `020-audit-hardening` | **Date**: 2026-06-29 | **Spec**: [specs/020-audit-hardening/spec.md](spec.md)
**Input**: Feature specification from `specs/020-audit-hardening/spec.md`

## Summary

Close the repository-wide security, OPC UA spec-compliance, CodeRabbit, manual review, and negative-path test audit findings before adding more feature surface. The technical approach is test-first and traceability-first: every future implementation task must be atomic, reference the exact OPC UA section and StatusCode it changes, name the RED test before production work, and be ordered so shared decoder/status/security foundations precede service-specific fixes and documentation evidence.

## Technical Context

**Language/Version**: C11 with freestanding-compatible core style
**Primary Dependencies**: Existing CMake build, Unity tests, CTest integration tests, sanitizer builds, libFuzzer harnesses, OPC UA reference documentation, OPC Foundation status/NodeSet tables
**Storage**: Static server, connection, session, subscription, dynamic address-space, history, and query storage; no mandatory heap allocation in protocol hot path
**Testing**: Unity unit tests, CTest integration tests, sanitizer-enabled CTest, fuzz harness build/run for parser and service dispatch inputs, interop smoke tests where applicable
**Target Platform**: Embedded microcontrollers represented by RP2040/Pico SDK and ARM Cortex-M0+ sizing, plus Linux host validation
**Project Type**: Embedded C OPC UA server library
**Performance Goals**: No heap on protocol hot path; malformed-input rejection remains bounded and linear in message length; valid minimal discovery/session/read flow latency must stay within 5% of the pre-change host validation measurement recorded for this feature; response construction remains within negotiated transport limits
**Constraints**: Preserve exact OPC UA StatusCodes and section citations; keep profile claims profile-targeting unless external CTT evidence exists; preserve caller-provided memory model; fail closed for missing security entropy; do not weaken bounds checks for size
**Scale/Scope**: Existing nano, micro, embedded, and full profiles; current host archive baseline is 152,709 bytes text, 6,224 bytes data, 0 bss for `build/src/libmicro_opcua.a`; optional full-profile services require profile-specific evidence
**OPC UA Normative References**: OPC-10000-4 §5.5.2.2 FindServers parameters; OPC-10000-4 §5.5.4.2 GetEndpoints parameters; OPC-10000-4 §5.6.2 OpenSecureChannel; OPC-10000-4 §5.7.2 CreateSession; OPC-10000-4 §5.7.3 ActivateSession; OPC-10000-4 §5.8.2 and §5.8.3 NodeManagement lifetime-sensitive inputs; OPC-10000-4 §5.9.2 Browse and §5.9.2.4 Browse StatusCodes; OPC-10000-4 §5.11.2 Read; OPC-10000-4 §5.11.3.2 HistoryRead parameters; OPC-10000-4 §5.11.4 Write; OPC-10000-4 §5.13.2.4 and §5.13.3.4 MonitoredItem StatusCodes; OPC-10000-4 §5.14 Publish/Subscription services; OPC-10000-4 §7.7.1 ContentFilter; OPC-10000-4 §7.9 ContinuationPoint; OPC-10000-4 Appendix B §B.2.3 QueryFirst and §B.2.4 QueryNext; OPC-10000-4 §7.22.1 MonitoringFilter; OPC-10000-4 §7.22.4 AggregateFilter; OPC-10000-4 §7.38.2 Common StatusCodes; OPC-10000-4 §7.39 TimestampsToReturn; OPC-10000-6 §5.2 Binary encoding; OPC-10000-6 §5.2.5 Arrays; OPC-10000-6 §5.2.2.15 ExtensionObject; OPC-10000-6 §6.7.2 MessageChunk; OPC-10000-6 §7.1.2.2 Message Header; OPC-10000-6 §7.1.2.3 Hello Message; OPC-10000-6 §7.1.2.4 Acknowledge Message; OPC-10000-13 §4.2.2.4, §4.2.2.9, §4.2.2.10 aggregate objects; OPC-10000-13 §5.4.3.5, §5.4.3.10, §5.4.3.11 aggregate calculations; OPC-10000-7 §4.2 and §4.3 profile/conformance-unit claims
**Target OPC UA Profile/Conformance Units**: Profile-targeting embedded server subset over OPC UA TCP Binary; keep service/facet claims scoped to verified behaviors and disabled optional profiles documented as unsupported
**Conformance Status Target**: Profile-targeting with traceable audit closure evidence; not profile-compliant or CTT-verified

## Embedded Size Budget

**Flash Impact**: Target net host/full-profile text growth under +8 KiB and nano/embedded profile growth under +4 KiB. Any larger increase requires size evidence and an explicit rationale tied to a High/P0/P1 audit finding.
**RAM Impact**: Default minimal profile should not require new static RAM beyond existing server/session/channel storage. Optional NodeManagement, History, Subscription, Query, and multi-connection fixes may add bounded storage only when the owning feature macro is enabled and must update `MU_*_STORAGE_BYTES` evidence.
**Heap Use**: None in protocol hot path; no new mandatory heap allocation. Platform crypto backends may retain existing optional heap behavior outside the core, but this feature must not expand it.
**Static Tables Added**: Prefer no runtime static tables. StatusCode tests may add test-only tables; any runtime table must be compact, reproducible, and size-reviewed.
**Transport Buffers**: Maintain `MU_MIN_CHUNK_SIZE` 8192-byte lower bound for server receive storage; ACK values must not exceed peer-requested limits per OPC-10000-6 §7.1.2.4. Oversize responses must be deterministic (`Bad_ResponseTooLarge` or TCP Error as applicable).
**Crypto Dependency Impact**: Use existing entropy/crypto adapter interfaces. No new mandatory crypto backend dependency; security-sensitive token and nonce freshness must fail closed if a configured secure path lacks entropy.

## Constitution Check

*GATE: Passed*

- **Spec Fidelity**: Plan cites exact OPC UA sections for malformed decoding, service parameters, filters, StatusCodes, TCP negotiation, MessageChunks, aggregate scope, and conformance claims. Unsupported behavior has required failure outcomes.
- **Embedded C Core**: Scope stays in the existing freestanding-compatible C core and adapter boundaries; no OS dependency is introduced into protocol parsing, dispatch, or status mapping.
- **Memory Model**: Plan preserves no-heap protocol hot path; lifetime fixes must use bounded owned storage or reject unsupported persistent data.
- **Minimal OPC UA Surface**: Scope repairs existing claimed/compiled surfaces and narrows unsupported claims; it does not add broad new services, transports, encodings, or companion-spec support.
- **Profile Research**: Profile status remains profile-targeting per OPC-10000-7 §4.2/§4.3, with CTT/profile-compliant claims explicitly out of scope.
- **Correctness Gates**: Future tasks must start with failing tests for malformed arrays, ExtensionObjects, chunks, sessions, filters, StatusCodes, and persistent-lifetime cases before production changes.
- **Security Honesty**: `SecurityPolicy#None` remains only for anonymous/non-production interoperability; username/password over None is disabled by default and security freshness fails closed.
- **Toolchain Discipline**: Plan uses existing CMake, Unity, CTest, sanitizer, fuzz, static-analysis, embedded cross-compile, and size-report flows.
- **Size Discipline**: Flash/RAM/stack/buffer impacts are budgeted and must be measured after implementation, with optional-profile RAM increases isolated behind feature macros.

## Project Structure

### Documentation (this feature)

```text
specs/020-audit-hardening/
├── spec.md
├── plan.md
├── research.md
├── data-model.md
├── quickstart.md
├── contracts/
│   └── audit-hardening-contract.md
├── checklists/
│   └── requirements.md
└── tasks.md             # Generated later by /speckit-tasks
```

### Source Code (repository root)

```text
include/
└── micro_opcua/
    ├── config.h         # Size/storage constants and compile-time profile knobs
    ├── status.h         # Exact StatusCode constants required by this feature
    └── server.h         # Public adapter and server API contracts if needed
src/
├── core/
│   ├── server.c         # TCP stream/chunk framing, ERR behavior, negotiated limits
│   ├── message_chunk.c  # Message type, abort, and continuation chunk validation
│   ├── message_chunk.h
│   ├── service_header.c # Session token, activation, and channel binding checks
│   ├── service_header.h
│   ├── service_dispatch.c
│   ├── status.c
│   └── tcp_connection.c # HEL/ACK negotiation behavior
├── encoding/
│   ├── binary_reader.c
│   ├── binary_datavalue.c
│   ├── binary_extension_object.c
│   ├── binary_query.c
│   └── binary_variant.c
├── services/
│   ├── browse.c
│   ├── discovery.c
│   ├── history.c
│   ├── node_management.c
│   ├── read.c
│   ├── secure_channel.c
│   ├── session.c
│   ├── subscription.c
│   └── write.c
├── security/
│   └── security_policy.c # User-token policy defaults for SecurityPolicy None
tests/
├── unit/
├── integration/
├── fixtures/
└── fuzz/
docs/
├── conformance/
├── size/
├── traceability/
└── validation/
scripts/
├── check_stack_usage.sh
├── measure_size.sh
└── size-report.sh
```

**Structure Decision**: Keep fixes in existing protocol, encoding, service, test, script, and documentation locations. Avoid broad file splitting in this feature unless a tiny helper removes repeated malformed-length or ExtensionObject boundary logic. The later `/speckit-tasks` output must order work by dependency: shared status/decoder contracts first, service-specific behavior second, security/session fixes third when dependent on status/entropy helpers, optional-profile lifetime fixes next, then fuzz/CI/docs/size evidence.

## Task Planning Rules for `/speckit-tasks`

- Each task must be atomic: one failing test, one production behavior, one documentation/evidence update, or one verification command.
- Each protocol task must cite the exact OPC UA section it implements or verifies, using the references listed in Technical Context.
- Test tasks must precede implementation tasks for parsing, serialization, service dispatch, StatusCodes, SecureChannel/session state, address-space lifetime, and security behavior.
- Shared helpers must be implemented before service-specific adopters only after at least one RED test proves the helper's required behavior.
- Tasks must be ordered so P0 malformed decoding and session/security failures unblock service-specific filter, lifetime, and documentation work.
- Tasks may be parallel only when their write sets are disjoint and they do not depend on the same shared helper or status constant.

## Complexity Tracking

No constitution check violations.
