# Data Model: Audit Hardening

## Protocol Request

**Represents**: A decoded OPC UA transport message or service request.

**Key fields**:

- Message type and chunk type
- Declared message size and negotiated size limits
- Service type identifier
- RequestHeader and authentication token
- Service parameters, arrays, strings, byte strings, ExtensionObjects, filters, and optional DataValues

**Validation rules**:

- OPC UA Binary arrays accept `-1` as null, `0` as empty, positive counts within configured limits, and reject values below `-1` with `Bad_DecodingError`.
- Declared lengths must be within the parent message or subreader boundary before any read or skip.
- Abort or non-final continuation chunks do not dispatch as complete service requests in single-chunk operation.
- Mandatory service fields must be fully decoded before state creation or callbacks.

## Protocol State

**Represents**: Server-owned state that persists beyond a request.

**Key fields**:

- TCP connection negotiation state
- SecureChannel id, token id, security policy, security mode, sequence validator, and key freshness state
- Session id, authentication token, activation state, bound channel, timeout, and server nonce
- Subscription, Publish request, monitored item, retransmission, query continuation, history, and dynamic address-space state

**Validation rules**:

- State is created only after all mandatory request fields validate.
- Session-bound service use requires an activated session bound to the active SecureChannel.
- Existing-but-unactivated sessions return `Bad_SessionNotActivated`; unknown/invalid sessions return `Bad_SessionIdInvalid`.
- Oversize Publish responses must either be split/reported deterministically or rejected without silently dropping parked requests.

## Security Credential Material

**Represents**: Values used to establish freshness, identity, channel ownership, and credential policy.

**Key fields**:

- SecureChannel id
- Security token id
- Session id
- Session authentication token
- Server nonce
- User identity token type and token policy
- Entropy provider result

**Validation rules**:

- Security-sensitive identifiers and nonces are non-predictable in non-test builds.
- Entropy failure fails closed for operations requiring fresh values.
- Username/password tokens are not advertised or accepted over `SecurityPolicy#None` by default.
- Identity token ExtensionObject bodies are parent-bound before parsing.

## Persistent Service Data

**Represents**: Decoded data used after the request buffer or stack frame that introduced it is no longer valid.

**Key fields**:

- Dynamic node BrowseName, DisplayName, NodeId, and references
- History continuation points and returned DataValues
- Subscription retransmission messages and notification data
- Query continuation point identifiers

**Validation rules**:

- Persistent data must be copied into bounded owned storage or rejected as unsupported.
- Stored strings, byte strings, and NodeIds must have configured maximum lengths and deterministic over-length failure.
- Later requests must not mutate previously stored service-visible data by overwriting shared receive buffers.

## Conformance Claim

**Represents**: A documented statement about supported OPC UA behavior.

**Key fields**:

- Claimed service/profile/facet/status/security behavior
- Exact OPC UA part and section reference
- Evidence link to tests, fixtures, interop, CTT, size, or validation docs
- Claim status: experimental, profile-targeting, profile-compliant, or CTT-verified

**Validation rules**:

- No profile-compliant or CTT-verified claim is allowed without matching external evidence.
- Scope-limited behavior must be labeled as scoped and must list unsupported options.
- Public numeric StatusCode and storage formulas must match current headers and measured evidence.

## Audit Finding Closure

**Represents**: Traceability from a discovered audit issue to a verified outcome.

**Key fields**:

- Finding id and source audit lane
- Severity and affected user story
- OPC UA section reference
- Closure status: fixed, out-of-scope because disabled profile, deferred with risk rationale
- RED test or verification evidence
- Size/RAM/stack impact evidence if behavior changed storage or code size

**Validation rules**:

- P0/P1 and High findings cannot close as deferred without explicit risk rationale.
- Every closure must name the verification command or artifact that proves it.
- Each closure must map to at least one functional requirement from `spec.md`.

## State Transitions

### Request Validation

1. Bytes received.
2. TCP/message/chunk header validated.
3. Service body decoded within declared and negotiated limits.
4. Service-specific parameters validated.
5. State mutation or callback allowed only after validation succeeds.
6. Response or TCP Error emitted with exact StatusCode.

### Session Lifecycle

1. No session.
2. Valid `CreateSession` creates a created session with fresh token and nonce.
3. Valid `ActivateSession` on the owning channel activates the session.
4. Session-bound services require activated session and matching channel.
5. Close, timeout, or channel loss invalidates or closes the session according to documented behavior.

### Finding Closure

1. Finding imported from audit.
2. RED test/evidence task proves current gap.
3. Implementation or documentation task changes behavior/claim.
4. Verification task passes and records evidence.
5. Closure matrix marks the finding fixed, out of scope, or explicitly deferred.
