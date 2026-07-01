# Contract: Stale-Old-Name Regression Guard

Implements FR-008 / SC-006. Mirrors the existing stale-claim pattern in
`tests/unit/test_conformance_docs.c` and `tests/unit/test_traceability_docs.c`
(both already walk documentation files at test time and fail the suite on a
forbidden substring), extended to a repo-wide sweep.

## Inputs

- The set of tracked files in the repository, excluding build output directories
  (`build*/`) and VCS metadata (`.git/`). **Explicitly includes `specs/001-023/**`
  — no directory-based exclusion** (see research.md Decision 6's correction and
  Decision 4: those directories get the same literal-string substitution applied
  and must scan clean too, matching spec.md SC-002).
- The forbidden-literal set from research.md Decision 1: `micro-opcua`,
  `micro_opcua`, `MICRO_OPCUA`, `MicroOpcUa`, `Micro-OPCUA` (case-sensitive, exact
  substring match — not a word-boundary regex, consistent with how the rename
  itself was applied).

## Behavior

- Scans every input file for any forbidden literal as an exact substring.
- On any match: reports the file path, line number, and matched literal, and fails
  (non-zero exit for a script; a failing `TEST_ASSERT_*` for a Unity test).
- On zero matches across the whole tree: passes.

## Allow-list resolution (open item carried from data-model.md)

The migration note describing this rename (FR-007) will, almost by necessity,
need to say something like "renamed from micro-opcua" to be legible as a migration
note at all. Task-level implementation MUST resolve this one of two ways, decided
during `/speckit-tasks`/`/speckit-implement`, not left ambiguous in the shipped
guard:

1. **Split-string technique**: the migration note spells the old name in a way
   that is human-legible but not a literal substring match (e.g. joining pieces,
   or using a code span with a zero-width separator) — the guard then truly has a
   zero-entry allow-list, which is simplest to reason about and matches Decision 6's
   "no allow-list" default.
2. **Path-based exclusion**: the guard explicitly excludes exactly one file (the
   migration note) by path. Simpler to write, but is a standing exception that must
   be justified and kept minimal — exactly one entry, never a directory-wide
   exclusion, and never extended to cover future stale references.

Either is acceptable; the guard's own source/config MUST make the chosen mechanism
and its scope obvious to a future reader (a bare hardcoded exclusion list buried in
the test with no comment is not acceptable).

## Verification

- Before this feature's rename is complete: the guard, run against the
  pre-rename tree, fails loudly (proves it actually detects the old name).
- After the rename: the guard passes against the full tree.
- A follow-up PR that reintroduces `micro_opcua` anywhere (e.g. a careless
  copy-paste of an old code sample) MUST fail CI via this guard.
