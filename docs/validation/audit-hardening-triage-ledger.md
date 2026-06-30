# Audit Hardening Triage Ledger

Feature: `020-audit-hardening`

This is the authoritative in-repository triage ledger for the Feature 020 audit
closure matrix. It assigns stable source finding IDs for the six audit lanes
named by `specs/020-audit-hardening/spec.md` FR-021 and SC-008.

The original external subagent transcripts were not present in the repository
when T003b searched for them. These IDs therefore do not claim to reproduce
external transcript IDs. They are repository-owned source IDs used to close and
re-check the feature without fabricating unavailable metadata.

## Severity Scale

- `High/P1`: security, memory-safety, state-mutation, or release-gate issue that
  could invalidate audit closure if left open.
- `Medium/P2`: protocol-conformance, exact-StatusCode, documentation-honesty, or
  resource-evidence issue that could mislead clients or maintainers.
- `Low/P3`: disabled-profile or scoped-claim issue whose current outcome is
  documented rather than implemented as broader service support.

## Finding Ledger

| Source finding ID | Closure matrix ID | Source audit lane | Severity / priority | Outcome |
|---|---|---|---|---|
| TDD-NEG-001 | AH-ARRAY-LENGTH-BOUNDS | TDD backlog / negative-path testing | High/P1 | Fixed - evidence recorded |
| TDD-NEG-002 | AH-DECLARED-LENGTH-BOUNDS | TDD backlog / negative-path testing | High/P1 | Fixed - evidence recorded |
| SPEC-CODE-001 | AH-MANDATORY-SERVICE-BODIES | spec-to-code compliance | High/P1 | Fixed - evidence recorded |
| CODEXSEC-001 | AH-TRANSPORT-CHUNK-NEGOTIATION | Codex Security / security-audit | High/P1 | Fixed - evidence recorded |
| SPEC-CODE-002 | AH-READ-BROWSE-ENUM-STATUS | spec-to-code compliance | Medium/P2 | Fixed - evidence recorded |
| REVIEW-001 | AH-WRITE-DATAVALUE-PRESENCE | manual reviewer | High/P1 | Fixed - evidence recorded |
| TDD-NEG-003 | AH-QUERY-COUNT-SAFETY | TDD backlog / negative-path testing | High/P1 | Fixed - evidence recorded |
| CODEXSEC-002 | AH-SECURITY-FRESHNESS | Codex Security / security-audit | High/P1 | Fixed - evidence recorded |
| CODEXSEC-003 | AH-SESSION-CHANNEL-BINDING | Codex Security / security-audit | High/P1 | Fixed - evidence recorded |
| SEC-AUD-001 | AH-NONE-USERNAME-POLICY | security-audit / Codex Security | High/P1 | Fixed - evidence recorded |
| SPEC-CODE-003 | AH-DISCOVERY-FILTERS | spec-to-code compliance | Medium/P2 | Fixed - evidence recorded |
| CODERABBIT-001 | AH-MONITORING-FILTER-STATUS | CodeRabbit | Medium/P2 | Fixed - evidence recorded |
| SPEC-CODE-004 | AH-AGGREGATE-SCOPE | spec-to-code compliance | Medium/P2 | Fixed - evidence recorded |
| REVIEW-002 | AH-PUBLISH-OVERSIZE | manual reviewer | Medium/P2 | Fixed - evidence recorded |
| SPEC-CODE-005 | AH-UNSUPPORTED-SERVICE-STATUS | spec-to-code compliance | Medium/P2 | Fixed - evidence recorded |
| SEC-AUD-002 | AH-SANITIZER-HISTORY-LIFETIME-ASAN | security-audit / TDD backlog | High/P1 | Fixed - evidence recorded |
| TDD-NEG-004 | AH-FUZZ-BINARY-TYPES-PLACEHOLDER | TDD backlog / negative-path testing | High/P1 | Fixed - evidence recorded |
| REVIEW-003 | AH-HOST-FULL-FLASH-BUDGET-OVER | manual reviewer | High/P1 | Fixed - evidence recorded |
| REVIEW-004 | AH-ARM-NANO-EMBEDDED-SIZE-BLOCKED | manual reviewer | High/P1 | Fixed - evidence recorded |
| REVIEW-005 | AH-PICO-MINIMAL-SERVER-BLOCKED | manual reviewer | High/P1 | Fixed - evidence recorded |
| REVIEW-006 | AH-PICO-STACK-EVIDENCE-BLOCKED | manual reviewer | High/P1 | Fixed - evidence recorded |
| REVIEW-007 | AH-STATIC-RAM-CALLER-STORAGE-DELTAS-INCOMPLETE | manual reviewer | Medium/P2 | Fixed - evidence recorded |
| REVIEW-008 | AH-HEAP-EVIDENCE-ADVISORY-ONLY | manual reviewer | Medium/P2 | Fixed - evidence recorded |
| PROCESS-001 | AH-AUTHORITATIVE-SIX-AGENT-IDS-MISSING | all six contract lanes | High/P1 | Fixed - evidence recorded |
| SPEC-CODE-006 | AH-NANO-OPTIONAL-SERVICES-DISABLED | spec-to-code compliance | Low/P3 | Profile-disabled - documented |
| SPEC-CODE-007 | AH-MICRO-EMBEDDED-TIER-DELTAS-DISABLED | spec-to-code compliance | Low/P3 | Profile-disabled - documented |
| SPEC-CODE-008 | AH-TRANSFER-SUBSCRIPTIONS-UNSUPPORTED | spec-to-code compliance | Low/P3 | Profile-disabled - documented |
| SPEC-CODE-009 | AH-DEFAULT-OPTIONAL-FULL-SERVICES-SCOPED | spec-to-code compliance | Low/P3 | Profile-scoped - documented |

## Closure Check

- Total source findings represented: 28.
- Six audit lanes represented: security-audit, Codex Security,
  spec-to-code compliance, CodeRabbit, manual reviewer, and TDD backlog.
- High/P1 rows open without deferral approval: 0.
- P0 rows open without deferral approval: 0.
- P1 rows open without deferral approval: 0.

All High/P1 rows are either `Fixed - evidence recorded` or are the process row
closed by this ledger. Disabled-profile and scoped-profile rows are not open
implementation findings; they are documented conformance-claim boundaries.
