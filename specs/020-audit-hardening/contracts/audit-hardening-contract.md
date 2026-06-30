# Contract: Audit Hardening

This contract defines the behavior and evidence expected from implementation tasks generated from this plan.

## Task Atomicity Contract

Each generated task MUST satisfy all of the following:

- Covers one behavior, one test artifact, one documentation artifact, or one verification command.
- Names the exact OPC UA section when the task affects protocol-visible behavior.
- Names the expected StatusCode or TCP Error when the task handles a negative path.
- Lists the primary file or artifact it will change.
- Is ordered after any shared helper/status/token/storage task it depends on.
- For production protocol changes, has a preceding RED test task that should fail before implementation.

## Required Ordering Contract

1. Status constants and exact-status tests needed by later work.
2. Shared malformed-array and bounded-ExtensionObject RED tests.
3. Shared decoder/bounds behavior.
4. Session, SecureChannel, entropy, and user-token policy RED tests.
5. Session, SecureChannel, entropy, and user-token policy behavior.
6. Service-specific Discovery, Read, Write, Browse, Subscription, Query, History, and NodeManagement RED tests.
7. Service-specific behavior fixes.
8. Fuzz harnesses and sanitizer/CI gate hardening.
9. Documentation, traceability, size, stack, and closure matrix evidence.

## Protocol Behavior Contract

| Finding Class | OPC UA Reference | Required Outcome |
|---------------|------------------|------------------|
| Array length below `-1` | OPC-10000-6 §5.2.5 | Reject with `Bad_DecodingError`; do not treat as empty/null |
| Truncated mandatory service body | OPC-10000-4 §5.7.2, §7.38.2; service-specific parameter sections | Reject before state creation or callbacks |
| ExtensionObject declared length out of bounds | OPC-10000-6 §5.2.2.15 | Reject before subreader access or skip |
| Invalid BrowseDirection | OPC-10000-4 §5.9.2.4, §7.38.2 | Return `Bad_BrowseDirectionInvalid` |
| Invalid TimestampsToReturn | OPC-10000-4 §7.39, §7.38.2 | Return `Bad_TimestampsToReturnInvalid` where applicable |
| Unsupported MonitoringFilter | OPC-10000-4 §5.13.2.4, §5.13.3.4, §7.22.1 | Return per-item filter StatusCode, not silent success |
| AggregateFilter scoped support | OPC-10000-4 §7.22.4; OPC-10000-13 §4.2.2.4, §4.2.2.9, §4.2.2.10, §5.4.3.5, §5.4.3.10, §5.4.3.11 | Support only verified Average/Minimum/Maximum subset or reject unsupported options |
| HEL/ACK buffer negotiation | OPC-10000-6 §7.1.2.3, §7.1.2.4 | Do not ACK buffer sizes larger than peer-requested values; fail deterministic unsupported negotiation |
| Message type/chunk finality | OPC-10000-6 §6.7.2, §7.1.2.2 | Invalid types, abort chunks, and non-final chunks do not dispatch service bodies |
| Existing unactivated session | OPC-10000-4 §7.38.2 | Return `Bad_SessionNotActivated` |
| Unknown/invalid session | OPC-10000-4 §7.38.2 | Return `Bad_SessionIdInvalid` |
| Plaintext username over None | OPC-10000-4 §5.7.3; security policy claim docs | Not advertised or accepted by default |

## Evidence Contract

Every audit finding closure MUST record:

- Audit source: security-audit, Codex Security, spec-to-code compliance, CodeRabbit, manual reviewer, or TDD backlog.
- Severity and priority.
- OPC UA section reference or non-protocol rationale.
- RED test or static evidence that failed before implementation.
- Final verification command and result.
- Size/RAM/stack impact if any production or storage behavior changed.
- Documentation or traceability update when a conformance claim changed.

## Verification Contract

Release validation for this feature requires:

- Normal host CTest passes.
- Sanitizer-enabled CTest passes.
- Malformed-input and exact-StatusCode regression tests pass.
- Fuzz targets for binary reader, message chunks, binary types, ActivateSession, and subscription filters build and have non-placeholder input coverage.
- Required CI/static-analysis/embedded-build gates are either blocking or explicitly documented as advisory with risk rationale.
- Size and stack evidence is updated for affected profiles.
