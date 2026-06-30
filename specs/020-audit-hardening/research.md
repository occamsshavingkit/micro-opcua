# Research: Audit Hardening

## Decision: Generate tasks as atomic, spec-grounded behavior slices

**Rationale**: The user explicitly requested tasks that are atomic, grounded in the OPC UA spec, and well ordered. The audits also showed that broad interop tests missed exact negative behavior. Every future task should therefore contain one behavior, one RED test or evidence artifact, one OPC UA section reference, and one clear owner file or doc surface.

**Alternatives considered**:

- Group by source file: rejected because it hides cross-service protocol rules such as array length handling.
- Group by audit agent: rejected because the same root cause appears in multiple audit reports.
- Group by severity only: rejected because shared helpers must precede some higher-level service fixes.

## Decision: Treat malformed arrays as the first shared decoder policy

**Rationale**: OPC-10000-6 §5.2.5 says a null array is encoded with length `-1`; the audit found multiple services accepting all negative values as empty arrays. This is the broadest wrong-success class and should be closed before service-specific work. Tasks should first add RED tests for representative services, then centralize or consistently apply the policy that `< -1` is `Bad_DecodingError`.

**Alternatives considered**:

- Patch each service ad hoc: rejected because repeated logic caused the audit finding and makes later review harder.
- Treat all negative values as `Bad_NothingToDo`: rejected because OPC UA Binary encoding reserves only `-1` for null arrays.

## Decision: Enforce bounded subreaders for ExtensionObjects and declared body lengths

**Rationale**: ActivateSession, History, AggregateFilter, and mutation DataValue paths all depend on declared length boundaries. OPC-10000-6 §5.2.2.15 defines ExtensionObject binary bodies with explicit encoding and length. Future tasks should prove declared bodies cannot read past the parent request, cannot consume following fields when too short, and cannot skip by overflowing position arithmetic.

**Alternatives considered**:

- Rely on parent reader sticky status only: rejected because subreaders created with attacker-declared lengths can lose the parent boundary.
- Reject all ExtensionObjects in optional services: rejected where the project already claims scoped support and can validate bounded bodies.

## Decision: Use exact service-level and operation-level StatusCodes

**Rationale**: OPC-10000-4 §7.38.2 distinguishes common StatusCodes, while service sections such as §5.13.2.4 and §5.13.3.4 require per-operation filter results. The earlier wrong-code issue passed interop because smoke tests only checked broad success/failure. Tasks must assert exact numeric StatusCodes for every negative path.

**Alternatives considered**:

- Return generic `Bad_DecodingError` for every invalid request: rejected where OPC UA defines a more specific invalid-parameter or per-operation result.
- Keep current client-tolerated results: rejected because the constitution prioritizes spec fidelity over broad client tolerance.

## Decision: Fail security freshness closed and bind sessions to channels

**Rationale**: Predictable session/channel tokens, late channel binding, and deterministic nonce fallback undermine Session and SecureChannel trust. Security-sensitive identifiers and nonces should come from the existing entropy adapter, and operations requiring freshness should fail if entropy is unavailable. Sessions should be bound to the creating/activating SecureChannel before cross-channel use is possible.

**Alternatives considered**:

- Keep deterministic values for tiny builds: rejected because security-sensitive behavior must not be weakened silently for size.
- Allow deterministic values only under a test macro: acceptable only for tests/examples if clearly isolated and never advertised as production security.

## Decision: Do not advertise or accept plaintext username/password over SecurityPolicy None by default

**Rationale**: SecurityPolicy None may remain for anonymous interoperability, but plaintext username/password credentials over None can be captured and replayed. This feature should make that path opt-in only if explicitly documented as unsafe interoperability.

**Alternatives considered**:

- Keep current plaintext behavior for interop: rejected because it is a security downgrade by default.
- Remove Username tokens entirely: rejected because secured username activation remains a valid scoped feature.

## Decision: Treat HEL/ACK negotiation as a conformance gate, not a size shortcut

**Rationale**: OPC-10000-6 §7.1.2.3 and §7.1.2.4 define Hello/Acknowledge buffer size rules, and §7.1.2.4 says ACK `SendBufferSize` must not be larger than the `ReceiveBufferSize` requested in Hello. The server must not silently raise a peer's requested value and advertise the raised value back. If the peer request is below what this implementation can support, fail deterministically rather than misrepresenting negotiated limits.

**Alternatives considered**:

- Keep up-negotiating to 8192: rejected because it can produce ACK values larger than the peer requested.
- Shrink server buffers below 8192 to save RAM: rejected because `MU_CONNECTION_RX_BUFFER_SIZE` is tied to the OPC UA minimum chunk-size floor and current multi-connection storage contract.

## Decision: Use bounded ownership for data that survives request decoding

**Rationale**: Dynamic NodeManagement names/references, History continuation points, retransmission data, and notification payloads may outlive stack or receive-buffer storage. Persistent service data must either be copied into bounded owned storage or rejected as unsupported if bounded storage cannot preserve it.

**Alternatives considered**:

- Store pointers into request buffers to save RAM: rejected because later requests can corrupt visible address-space state or response data.
- Allocate from heap when optional services are enabled: rejected because the constitution requires caller-provided/static memory for protocol hot paths.

## Decision: Make verification gates honest before release validation

**Rationale**: Advisory CI steps and grep-based conformance checks can false-green. This feature must distinguish required gates from explicitly advisory checks and must ensure conformance docs, StatusCode values, size/stack budgets, sanitizer tests, fuzz build/run, and embedded cross-compile evidence are auditable.

**Alternatives considered**:

- Keep advisory-only gates for developer convenience: rejected for required release validation, but acceptable for separately labeled exploratory checks.
- Depend only on .NET/asyncua interop: rejected because those paths did not catch exact negative StatusCodes.

## Decision: Keep aggregate support scoped rather than expanding to full Part 13

**Rationale**: OPC-10000-4 §7.22.4 defines AggregateFilter and OPC-10000-13 defines full aggregate semantics. The audited implementation targets Average, Minimum, and Maximum only. This feature should either reject unsupported aggregate configuration/status semantics with exact StatusCodes or document scoped support; it should not expand into full Part 13 conformance unless a future spec explicitly asks for it.

**Alternatives considered**:

- Implement full aggregate configuration now: rejected as broad feature expansion beyond audit hardening.
- Leave docs as "implemented": rejected because it can overclaim conformance.
