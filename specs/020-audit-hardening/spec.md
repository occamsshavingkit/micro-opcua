# Feature Specification: Audit Hardening

**Feature Branch**: `020-audit-hardening`
**Created**: 2026-06-29
**Status**: Draft
**Input**: User description: "Fix all audit findings from the repository-wide security, OPC UA spec compliance, code review, CodeRabbit, and negative-path test audits: malformed decoding, exact OPC UA StatusCodes, session and SecureChannel security, buffer negotiation, filter validation, lifetime ownership, fuzz/CI/doc guardrails, and conformance honesty."

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Reject Malformed OPC UA Traffic Deterministically (Priority: P1)

An OPC UA client, conformance tool, or malicious peer sends malformed, truncated, or boundary-case messages. The server rejects each invalid message with the exact OPC UA StatusCode or TCP Error required for that situation, does not create or consume protocol state, and does not call application handlers with invalid data.

**Why this priority**: This closes the highest-risk class found by the audits: malformed input currently succeeds in several paths, which can consume sessions, mislead clients, or expose uninitialized data to application callbacks.

**Independent Test**: Can be fully tested by sending malformed requests for Discovery, Session, Attribute, Browse, Subscription, Query, and History services and verifying the cited rejection result plus no state mutation.

**Acceptance Scenarios**:

1. **Given** a request with an array length below the OPC UA null-array sentinel, **When** the server decodes the request, **Then** the request is rejected with `Bad_DecodingError` and the service state is unchanged.
2. **Given** a truncated `CreateSession` request that contains only a valid header or partial body, **When** the server processes it, **Then** no session is created and the response reports `Bad_DecodingError`.
3. **Given** an ExtensionObject body length that exceeds the remaining request body or is shorter than required fields, **When** the server processes the request, **Then** decoding fails before any service callback or state transition.
4. **Given** an invalid service enum such as an unsupported timestamp selection or browse direction, **When** the relevant service is processed, **Then** the service returns the exact OPC UA invalid-parameter StatusCode.

---

### User Story 2 - Preserve Session, Channel, and Credential Security (Priority: P1)

An integrator deploys the server in a multi-client or secured environment. Session identifiers, authentication tokens, SecureChannel identifiers, token identifiers, server nonces, and user identity handling prevent guessing, cross-channel activation, replay, and plaintext credential exposure by default.

**Why this priority**: Predictable tokens, weak channel binding, deterministic nonce behavior, and username tokens over `SecurityPolicy#None` can let a peer confuse sessions or expose credentials.

**Independent Test**: Can be tested by opening multiple channels and sessions, attempting cross-channel activation and service use, forcing entropy failure, replaying identity tokens, and inspecting advertised endpoint user-token policies.

**Acceptance Scenarios**:

1. **Given** two simultaneous clients, **When** one client guesses or reuses the other client's session token, **Then** activation and session-bound services are rejected.
2. **Given** entropy is unavailable for security-sensitive nonces or tokens, **When** a SecureChannel or Session operation requires fresh values, **Then** the operation fails closed rather than using a predictable fallback.
3. **Given** an endpoint uses `SecurityPolicy#None`, **When** user-token policies are advertised and activated by default, **Then** plaintext username/password tokens are not accepted or advertised.

---

### User Story 3 - Match OPC UA Service Semantics for Claimed Features (Priority: P2)

An OPC UA client relies on advertised Discovery, Session, Attribute, Browse, Subscription, Aggregate, Query, History, and NodeManagement behavior. Each claimed service either behaves according to the cited OPC UA section or explicitly rejects unsupported options with the correct StatusCode.

**Why this priority**: The audits found several features that were partly implemented, silently accepted unsupported options, or were documented more broadly than the implemented behavior.

**Independent Test**: Can be tested service-by-service by exercising supported and unsupported options and comparing each response to the cited OPC UA section and conformance claim.

**Acceptance Scenarios**:

1. **Given** a `FindServers` request with filters that do not match this server, **When** the server responds, **Then** it returns no matching servers rather than ignoring the filters.
2. **Given** `CreateMonitoredItems` or `ModifyMonitoredItems` contains an unsupported MonitoringFilter, **When** the operation is processed, **Then** the per-item result reports the appropriate filter StatusCode and no unsupported monitored item behavior is enabled.
3. **Given** AggregateFilter options outside the documented scoped support, **When** a client requests them, **Then** the server either implements the claimed semantics or rejects them and keeps conformance claims scoped.
4. **Given** NodeManagement or History services are enabled, **When** data from a request must survive after decoding, **Then** later Browse, Read, or History responses remain stable after subsequent traffic.

---

### User Story 4 - Keep Conformance Claims and Verification Honest (Priority: P3)

A maintainer or integrator reads project documentation and verification output. The documented conformance level, service matrix, StatusCode values, size impact, stack budget, and automated verification status accurately reflect what is implemented and tested.

**Why this priority**: Incorrect documentation and advisory-only gates let regressions pass despite green user-facing interop checks.

**Independent Test**: Can be tested by reviewing generated evidence, running verification gates, and checking that documentation claims cite exact OPC UA sections and match observed behavior.

**Acceptance Scenarios**:

1. **Given** documentation claims support for a service, profile, facet, StatusCode, or aggregate behavior, **When** the claim is checked, **Then** it cites the exact OPC UA section and has matching tests or is clearly labeled experimental/profile-targeting.
2. **Given** automated verification detects malformed input, memory-safety, conformance-doc, or embedded-build regression in a required gate, **When** a release validation is run, **Then** the regression blocks the validation unless it is explicitly documented as advisory.
3. **Given** a size or stack budget changes, **When** a maintainer reviews the feature, **Then** the flash, RAM, stack, and application-headroom impact is documented with before/after evidence.

### Edge Cases

- Messages with array lengths of `-2` or lower are malformed; only `-1` is treated as the OPC UA null-array sentinel.
- Declared strings, byte strings, ExtensionObjects, MessageChunks, and service bodies may be truncated, over-declared, exactly at the configured limit, or larger than negotiated limits.
- Continuation chunks, abort chunks, invalid TCP message types, and oversized chunks must not dispatch service bodies in single-chunk operation.
- Optional DataValue fields may be absent; mutation services must not infer or use absent values.
- Entropy providers may fail, repeat bytes, or be unavailable on embedded targets.
- Optional services may be disabled, enabled for profile testing, or present only as experimental features; each state must have an honest claim and deterministic unsupported behavior.
- Dynamic names, NodeIds, continuation points, and notification payloads may outlive the request buffer that carried them.
- Verification tools may detect no matches, actual failures, or tool execution errors; release evidence must distinguish these outcomes.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: The server MUST reject malformed OPC UA array lengths below `-1` with `Bad_DecodingError` for every decoded service parameter array in the supported service surface.
- **FR-002**: The server MUST fully validate mandatory request bodies before creating protocol state, including `CreateSession`, `FindServers`, `GetEndpoints`, `Read`, `Write`, `Browse`, Subscription, Query, and History requests.
- **FR-003**: The server MUST enforce declared ExtensionObject, string, byte string, message body, MessageChunk, and negotiated buffer boundaries before reading, skipping, storing, or exposing decoded data.
- **FR-004**: The server MUST reject mutation requests that require a value when the transmitted DataValue omits the value field, and MUST NOT call application handlers with absent or uninitialized value fields.
- **FR-005**: The server MUST generate non-predictable SecureChannel identifiers, token identifiers, Session identifiers, authentication tokens, and server nonces for security-sensitive use, using fail-closed behavior when freshness cannot be provided.
- **FR-006**: The server MUST bind each Session to the SecureChannel that created and activated it, and MUST reject cross-channel activation or session-bound service use.
- **FR-007**: The server MUST return `Bad_SessionNotActivated` when an existing Session is used before activation, and `Bad_SessionIdInvalid` only when the Session identity is invalid or unknown.
- **FR-008**: The server MUST NOT advertise or accept plaintext username/password user identity tokens over `SecurityPolicy#None` by default.
- **FR-009**: The server MUST reject or handle Hello/Acknowledge negotiation so that advertised send and receive buffer sizes comply with peer-requested limits and OPC UA minimum-size rules.
- **FR-010**: The server MUST reject invalid TCP message types and MUST NOT dispatch abort or non-final continuation chunks as complete service requests unless chunk reassembly support is explicitly implemented and claimed.
- **FR-011**: Discovery services MUST decode and honor the request filters defined for each service: `FindServers` endpoint URL, locale, server URI, and server type filters; and `GetEndpoints` endpoint URL, locale, and transport profile filters.
- **FR-012**: Read, Browse, MonitoredItem, Subscription, Query, and History services MUST validate service enums and option fields and return the exact OPC UA StatusCode for invalid values.
- **FR-013**: `CreateMonitoredItems` and `ModifyMonitoredItems` MUST return per-item filter StatusCodes for unsupported, invalid, or disallowed MonitoringFilters, and MUST NOT silently treat unsupported filters as successful.
- **FR-014**: Scoped AggregateFilter support MUST be limited to the documented Average, Minimum, and Maximum behavior that is actually verified; unsupported aggregate configuration or full Part 13 semantics MUST be rejected or documented as out of scope.
- **FR-015**: Data that survives beyond request decoding, including dynamic address-space names, string NodeIds, references, history continuation points, and retransmission data, MUST remain stable after later requests and MUST NOT alias transient request or stack storage.
- **FR-016**: Publish handling MUST respond deterministically when a notification response cannot fit within the supported response size; it MUST NOT silently drop parked Publish requests or notification data.
- **FR-017**: Query and filter decoding MUST use overflow-safe count validation suitable for 32-bit embedded targets before assigning fixed-capacity result or operand storage.
- **FR-018**: Conformance documentation MUST cite exact OPC UA sections for every protocol claim and MUST label each claim as experimental, profile-targeting, profile-compliant, or CTT-verified.
- **FR-019**: Public documentation MUST report current StatusCode numeric values, multi-connection storage requirements, size impact, stack-budget scope, and unsupported-service behavior accurately.
- **FR-020**: Required verification gates MUST detect regressions in malformed-input handling, exact StatusCodes, conformance documentation, memory-safety behavior, formatting/static-analysis, and embedded build readiness before release validation succeeds.
- **FR-021**: Every confirmed audit finding imported from the security, Codex Security, OPC UA compliance, CodeRabbit, manual review, and negative-path testing passes MUST have closure evidence with source id, source audit lane, severity, and outcome: fixed, explicitly out of scope for disabled profiles, or documented as a follow-up with a risk rationale.

### OPC UA Normative Scope *(mandatory for protocol features)*

- **OPC-001**: Target OPC UA role/profile assumption remains an embedded OPC UA server over `opc.tcp` with OPC UA Binary encoding, profile-targeting only until external CTT evidence exists. Conformance-unit and profile claims are governed by OPC-10000-7 §4.2 and §4.3.
- **OPC-002**: Discovery requirements cover OPC-10000-4 §5.5.2 `FindServers` and §5.5.4 `GetEndpoints`.
- **OPC-003**: SecureChannel and Session requirements cover OPC-10000-4 §5.6.2 `OpenSecureChannel`, §5.7.2 `CreateSession`, §5.7.3 `ActivateSession`, and OPC-10000-6 §6.7.2 MessageChunk structure.
- **OPC-004**: Attribute, Browse, Write, Subscription, Publish, Query, History, and NodeManagement requirements cover OPC-10000-4 §5.8.2, §5.8.3, §5.9, §5.11.2, §5.11.3.2, §5.11.4, §5.13, §5.14, §7.7.1, §7.9, Appendix B §B.2.3, and Appendix B §B.2.4 for enabled features.
- **OPC-005**: MonitoringFilter and AggregateFilter behavior cites OPC-10000-4 §5.13.2.4, §5.13.3.4, §7.22.1, and §7.22.4; scoped Average, Minimum, and Maximum aggregate behavior cites OPC-10000-13 §4.2.2.4, §4.2.2.9, §4.2.2.10, §5.4.3.5, §5.4.3.10, and §5.4.3.11.
- **OPC-006**: Binary encoding and transport validation cites OPC-10000-6 §5.2, §7.1.2.2, §7.1.2.3, §7.1.2.4, and §7.2.
- **OPC-007**: StatusCode requirements cite OPC-10000-4 §7.38.2 and include at minimum `Bad_DecodingError`, `Bad_MonitoredItemFilterUnsupported`, `Bad_MonitoredItemFilterInvalid`, `Bad_FilterNotAllowed`, `Bad_TimestampsToReturnInvalid`, `Bad_BrowseDirectionInvalid`, `Bad_SessionNotActivated`, `Bad_SessionIdInvalid`, `Bad_ResponseTooLarge`, `Bad_TcpMessageTypeInvalid`, and relevant security rejection StatusCodes.
- **OPC-008**: Unsupported services, disabled optional profiles, unsupported filters, unsupported aggregate options, and unsupported security modes MUST fail with cited OPC UA StatusCodes rather than silent success.
- **OPC-009**: SecurityPolicy and conformance status remain experimental or profile-targeting unless specific CTT evidence is added; `SecurityPolicy#None` may remain for anonymous interoperability but not for plaintext username/password credentials by default.

### Scope Boundaries *(mandatory)*

- **In Scope**: All confirmed audit findings from the six-agent audit pass: malformed decoding, request-body completeness, exact StatusCodes, session/channel/token/nonce security, user-token policy defaults, Discovery filters, HEL/ACK negotiation, MessageChunk finality, MonitoringFilter and AggregateFilter validation, DataValue presence, Query count safety, History and NodeManagement lifetime ownership, Publish oversize behavior, conformance documentation, size/stack evidence, and verification gates.
- **Out of Scope**: Full CTT certification, a broad new OPC UA service surface, alternate transports or encodings, full OPC-10000-13 aggregate coverage beyond the explicitly scoped Average/Minimum/Maximum behavior, companion specifications, and replacement of the project's embedded memory model.
- **Compatibility Claim**: After this feature ships, the project may claim only profile-targeting support for the verified embedded server subset over `opc.tcp` with OPC UA Binary encoding, plus explicitly listed services and filters that have matching evidence. It must not claim profile-compliant or CTT-verified status without external evidence.
- **Application Headroom Goal**: The default embedded configuration must satisfy the plan budget of net host/full-profile text growth under +8 KiB, nano/embedded profile text growth under +4 KiB, no new default static RAM beyond existing server/session/channel storage, no heap use on the protocol hot path, and recorded stack evidence for all hardening changes.

### Key Entities

- **Protocol Request**: A client message or service request containing encoded fields, arrays, ExtensionObjects, chunks, filters, security tokens, and optional values that must be validated before use.
- **Protocol State**: Server-side state created or consumed by transport, SecureChannel, Session, Subscription, Publish, History, Query, or NodeManagement behavior.
- **Security Credential Material**: Nonces, channel identifiers, token identifiers, authentication tokens, user identity tokens, and token policies used to authenticate or bind client activity.
- **Persistent Service Data**: Names, NodeIds, references, continuation points, retransmission messages, and notification payloads that may outlive the request body.
- **Conformance Claim**: A documented statement about supported profiles, services, StatusCodes, filters, security policies, or verification status with an exact OPC UA citation and evidence.
- **Audit Finding Closure**: A traceable outcome for each audit finding: fixed and verified, profile-disabled and honestly documented, or deferred with a recorded risk rationale.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: 100% of specified malformed-input scenarios return the cited OPC UA StatusCode or TCP Error, and no malformed scenario produces silent success.
- **SC-002**: 100% of state-creation and application-callback negative tests prove no Session, Subscription, dynamic address-space data, History data, or application callback is created or consumed before validation succeeds.
- **SC-003**: Multi-client security tests demonstrate that session tokens cannot be guessed from fixed values, sessions cannot be activated or used from the wrong channel, and nonce/token freshness failures are rejected.
- **SC-004**: Discovery, Session, Read, Write, Browse, Subscription, Publish, Query, History, and NodeManagement negative-path tests cover every P0 and P1 audit finding with exact expected StatusCodes.
- **SC-005**: All conformance documentation and traceability rows for affected behavior cite exact OPC UA sections and contain no unsupported profile-compliant or CTT-verified claims.
- **SC-006**: Release validation includes passing normal host tests, sanitizer-enabled tests, malformed-input regression tests, conformance-document checks, formatting/static-analysis checks, and embedded build readiness checks for required gates.
- **SC-007**: Size evidence documents flash, RAM, and stack impact for the hardening changes and shows the default embedded configuration remains within the application-headroom budget: net host/full-profile text growth under +8 KiB, nano/embedded profile text growth under +4 KiB, no new default static RAM beyond existing server/session/channel storage, and no heap use on the protocol hot path.
- **SC-008**: Every confirmed audit finding is represented in a closure matrix with source id, source audit lane, evidence, severity, and outcome, and no High or P0/P1 finding remains open without explicit deferral approval.

## Assumptions

- The final triage from the six-agent audit pass is the authoritative input for this feature's finding list.
- Optional services must be conformant when enabled; when disabled, documentation must clearly avoid claiming unsupported behavior.
- `SecurityPolicy#None` may remain available for anonymous interoperability and non-production testing, but plaintext username/password credentials are not accepted over it by default.
- Full OPC UA CTT certification is not part of this feature; the expected outcome is profile-targeting honesty plus test-backed behavior for the claimed subset.
- The embedded no-heap, bounded-buffer, caller-provided memory model remains a project constraint.
- Any finding that would require broad feature expansion may be resolved by rejecting unsupported options with the correct StatusCode and narrowing documentation, provided the user-visible claim is honest.
