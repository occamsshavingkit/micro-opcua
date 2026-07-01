# Feature Specification: Rename micro-opcua to muc-opcua

**Feature Branch**: `024-rename-muc-opcua`
**Created**: 2026-07-01
**Status**: Draft
**Input**: User description: "Rename the project from micro-opcua to muc-opcua across the entire repository. The name change reflects that the library targets microcontrollers (µc = \"mu-c\"), the maintainer's location (MUC airport code, Munich), and avoids confusion with the OPC UA \"Micro\" server profile the library also implements. Scope: full identifier rename covering MICRO_OPCUA_* macros/CMake options -> MUC_OPCUA_*, include/micro_opcua/ -> include/muc_opcua/ (including the umbrella header), every #include of \"micro_opcua/...\" across src/tests/platform/examples, CMake project/library/target/package names, all documentation (README, CLAUDE.md, AGENTS.md, ROADMAP.md, docs/**, specs/**/*.md living references), CI workflow files/badges/URLs. The GitHub repository has ALREADY been renamed (occamsshavingkit/micro-opcua -> occamsshavingkit/muc-opcua) and the local origin remote already points at the new URL. Whether the mu_/opcua_ C function/type prefixes change is an open decision for planning, leaning toward keeping them unchanged. This is a breaking change; no published stable release exists yet, so no compatibility shim is required unless cheap. Non-goals: do not rewrite specs/001-023 content beyond literal name/path/macro references needed so historical docs don't lie about the current build system."

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Integrator builds the renamed library from a fresh clone (Priority: P1)

An embedded integrator clones `github.com/occamsshavingkit/muc-opcua`, follows the README, and builds the host and embedded profiles exactly as documented, using `muc_opcua` include paths, `MUC_OPCUA_*` CMake options, and `muc_opcua.h`.

**Why this priority**: If the build system, headers, and README are inconsistent, no other part of the rename matters — the library simply won't build or will build under a name that doesn't match what a new reader was told.

**Independent Test**: Clone the repository fresh, follow `README.md` verbatim (CMake configure/build/test commands), and confirm the build succeeds using only `MUC_OPCUA_*` option names and `include/muc_opcua/*.h` headers, with zero references to `MICRO_OPCUA_*` or `include/micro_opcua/` remaining in any file that is part of the current (non-historical) build.

**Acceptance Scenarios**:

1. **Given** a fresh clone of the renamed repository, **When** an integrator configures the project with CMake using the documented options, **Then** every option is named `MUC_OPCUA_*` and the configure step succeeds with no reference to the old `MICRO_OPCUA_*` names.
2. **Given** the renamed public header tree, **When** any source file under `src/`, `tests/`, `platform/`, or `examples/` includes a library header, **Then** it includes `"muc_opcua/...".h` (or the umbrella `muc_opcua.h`), and no file includes `"micro_opcua/...".h`.
3. **Given** the built library, **When** an integrator inspects the produced CMake target/package name, **Then** it is named consistently with `muc_opcua` (not `micro_opcua`), including in any `find_package`/`target_link_libraries` usage shown in docs.

---

### User Story 2 - Reader trusts current documentation and links (Priority: P1)

A new or returning reader opens the README, CLAUDE.md, AGENTS.md, ROADMAP.md, or any page under `docs/` and sees the project consistently referred to as `muc-opcua`, with working links back to `github.com/occamsshavingkit/muc-opcua` and no stray references to the old name that would make them doubt they're looking at current documentation.

**Why this priority**: Immediately after User Story 1 in importance — a rename that leaves the docs half-migrated is worse than no rename, because it actively confuses readers about which name is authoritative, exactly the kind of stale-claim problem the project's existing conformance-doc guards (spec 019/023) already treat as a defect class.

**Independent Test**: Grep the full documentation set (README, CLAUDE.md, AGENTS.md, ROADMAP.md, everything under `docs/`) for the literal strings `micro-opcua`, `micro_opcua`, and `MICRO_OPCUA` and confirm zero matches outside of files that are frozen historical record: `specs/001` through `specs/023`, and the per-feature `docs/traceability/NNN-*.md` files that document those same already-shipped features. These are left completely untouched by this feature, the same way past git commits and already-merged PR descriptions aren't rewritten when a project renames itself.

**Acceptance Scenarios**:

1. **Given** the renamed repository, **When** a reader opens `README.md`, **Then** the project name, badges, clone URL, and any code samples all say `muc-opcua`/`muc_opcua`/`MUC_OPCUA` consistently.
2. **Given** any file under `docs/`, **When** it references the repository URL or library/macro names, **Then** it points at `github.com/occamsshavingkit/muc-opcua` and uses the new identifier names — except the per-feature `docs/traceability/NNN-*.md` files, which are historical record (see Scope Boundaries).
3. **Given** `specs/001` through `specs/023` and the per-feature `docs/traceability/NNN-*.md` files, **When** a maintainer reads them after the rename, **Then** they are byte-for-byte unchanged — including any old macro name, include path, or repo URL they mention — exactly as a past git commit or merged PR description isn't retroactively edited when a project renames itself.

---

### User Story 3 - CI and GitHub-facing metadata reflect the new name (Priority: P2)

A maintainer viewing GitHub Actions runs, the repository's badges, or any CI configuration sees the workflows still passing and referencing `muc-opcua`/`muc_opcua` consistently, with no broken badge links or CI steps still hardcoding the old macro/option names.

**Why this priority**: Lower risk than User Stories 1-2 because CI misconfiguration is caught automatically (a failing build is visible), but still needs to be done as part of the same migration so the "green CI" signal keeps meaning what it always has.

**Independent Test**: Run the full CI workflow set locally (or inspect them statically) and confirm every job references `MUC_OPCUA_*` CMake options and passes; confirm README/other doc badges point at the renamed repository's Actions URLs.

**Acceptance Scenarios**:

1. **Given** the renamed repository, **When** the CI workflows run (host build, sanitizer build, pico cross-compile, static analysis/fuzz), **Then** every step that previously referenced a `MICRO_OPCUA_*` CMake option now references the `MUC_OPCUA_*` equivalent and the jobs pass.
2. **Given** README badges, **When** a reader clicks a CI status badge, **Then** it resolves to the correct workflow run under `github.com/occamsshavingkit/muc-opcua`.

---

### Edge Cases

- What happens when a file legitimately needs to describe the *history* of the rename itself (e.g., this spec, or a future CHANGELOG entry) — it must be allowed to mention `micro-opcua`/`MICRO_OPCUA` as a historical fact without tripping any "no stale name" guard added by this feature.
- How does the project handle the case where `specs/001` through `specs/023` (and the per-feature `docs/traceability/NNN-*.md` files describing those same features) contain code snippets or exact file paths that literally quote old macro names — these are frozen historical record, exactly like a past git commit or an already-merged PR description, and are left completely untouched by this feature. No literal-reference "fix-up" is applied to them at all, even for build references a reader might otherwise copy-paste; the regression guard (FR-008) excludes them from its scan for the same reason.
- What happens if a downstream fork or clone already depends on `MICRO_OPCUA_*` macros or `include/micro_opcua/`? (No compatibility shim is required per the input; this must be stated plainly wherever the breaking change is documented, e.g. a CHANGELOG or migration note.)
- How does the project prevent this rename from silently regressing (someone reintroducing a `micro_opcua` reference in a later PR)? An automated guard (in the spirit of the existing `test_conformance_docs.c`/`test_traceability_docs.c` stale-claim checks) is expected.
- What happens to the embedded size/RAM ledger numbers recorded under the old macro names in `docs/size/*`? They must still be legible as historical measurement snapshots after any macro-name text in their surrounding prose is corrected.
- How does the rename avoid corrupting the OPC Foundation's own **Micro** server-profile vocabulary, which the project's rename is explicitly motivated by (per the Input above)? The `MUC_OPCUA_PROFILE` CMake option's accepted *values* (`nano`/`micro`/`embedded`/`full`/`custom`), the `make micro` build target, and every "Micro [Embedded Device 2017 Server] Profile" prose reference in `docs/conformance/*` and elsewhere are a bare, unprefixed `micro` — never the literal compound string `micro_opcua`/`MICRO_OPCUA`/`micro-opcua` — and MUST remain completely untouched by this rename.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: The build system MUST expose every current `MICRO_OPCUA_*` CMake/preprocessor option under the equivalent `MUC_OPCUA_*` name, with no functional change in behavior per option.
- **FR-002**: The public header tree MUST be relocated from `include/micro_opcua/` to `include/muc_opcua/`, including renaming the umbrella header from `micro_opcua.h` to `muc_opcua.h`, and every `#include` referencing the old path across `src/`, `tests/`, `platform/`, and `examples/` MUST be updated to the new path.
- **FR-003**: The CMake project name, library/target name(s), and any package/install metadata that currently encode `micro_opcua`/`micro-opcua` MUST be renamed to the `muc_opcua`/`muc-opcua` equivalents.
- **FR-004**: All current, non-historical documentation (`README.md`, `CLAUDE.md`, `AGENTS.md`, `ROADMAP.md`, everything under `docs/`) MUST refer to the project as `muc-opcua` (repo/branding) and `muc_opcua`/`MUC_OPCUA_*` (code/macros), with no remaining references to the old names.
- **FR-005**: CI workflow files and any badges/links they produce MUST reference the renamed repository and the `MUC_OPCUA_*` option names, and all CI jobs that passed before the rename MUST continue to pass after it.
- **FR-006**: Historical spec files `specs/001-*` through `specs/023-*`, and the per-feature `docs/traceability/NNN-*.md` files documenting those same already-shipped features, MUST NOT be edited at all by this feature — not their narrative, and not their literal macro-name/include-path/repository-URL references either. They are frozen historical record, exactly like already-merged commits and PR descriptions are never retroactively rewritten when a project renames itself.
- **FR-007**: The project MUST document, in a single clearly-linked location (e.g. a CHANGELOG/migration note), that this is a breaking rename with no compatibility shim, and MUST state exactly which identifiers changed (macro prefix, include path, CMake names) so an existing integrator can mechanically update their own build.
- **FR-008**: The project MUST add or extend an automated check (mirroring the existing stale-claim doc tests) that fails the test suite if a non-historical, non-changelog file reintroduces `micro-opcua`, `micro_opcua`, or `MICRO_OPCUA_` as a literal string. The check's scope explicitly excludes `specs/001-023/**` and the per-feature `docs/traceability/NNN-*.md` files (FR-006) — they are expected to retain the old name forever, by design.
- **FR-009**: The GitHub repository rename (`occamsshavingkit/micro-opcua` -> `occamsshavingkit/muc-opcua`) is already complete and the local `origin` remote already points at the new URL; this feature's scope is the code/documentation migration to match, not the repository rename itself.
- **FR-010**: Whether the `mu_`/`opcua_` C function and type prefixes (e.g. `mu_server_init`, `mu_nodeid_t`, `opcua_statuscode_t`) change as part of this rename is decided during planning (`/speckit-plan`), not in this spec; the default assumption carried into planning is that they remain unchanged, since `mu_` already reads as the start of `muc` and changing every public symbol would multiply the size of this already-large change for limited reader benefit.

### OPC UA Normative Scope *(mandatory for protocol features)*

- **OPC-001**: This feature changes no OPC UA wire behavior, StatusCode, NodeId, service, or profile-conformance claim. Target status remains **profile-targeting** (no profile-compliant/CTT-verified claim), unchanged from `docs/conformance/status.md` as of 2026-07-01.
- **OPC-002**: No services/features are added, removed, or altered in behavior by this rename; every conformance-unit row already recorded in `docs/conformance/*.md` continues to describe the same implemented behavior, just under the renamed identifiers/paths where those docs reference source locations.
- **OPC-003**: Unsupported-behavior StatusCode citations in existing docs are unchanged; this feature only touches identifier/path/name text, not decoder or dispatch logic.
- **OPC-004**: Wire encoding/transport requirements (OPC-10000-6) are unaffected; no chunk framing, encoding, or transport-profile URI changes.
- **OPC-005**: SecurityPolicy and conformance status remain exactly as documented pre-rename (profile-targeting); this feature must not accidentally introduce or remove a compliance claim while editing conformance docs for the name change.

### Scope Boundaries *(mandatory)*

- **In Scope**: Renaming `MICRO_OPCUA_*` macros/CMake options to `MUC_OPCUA_*`; relocating `include/micro_opcua/` to `include/muc_opcua/` and updating every internal `#include`; renaming CMake project/library/target/package identifiers; updating all current (non-historical) documentation and CI configuration to the new repo/library name; adding a stale-old-name regression guard (scoped to exclude frozen historical files); recording the breaking-change/migration note.
- **Out of Scope**: Renaming the GitHub repository itself (already done, out of band, before this spec). Editing `specs/001` through `specs/023`, or the per-feature `docs/traceability/NNN-*.md` files describing those same features, **at all** — not narrative, not literal build references either; they are frozen historical record (FR-006), the same way past git commits aren't rewritten when a project renames itself. Changing the `mu_`/`opcua_` C symbol prefixes (deferred to a planning-time decision, default: no change). Adding a backward-compatibility shim/alias layer for the old macro names or include path (not required per input, since no stable release exists; only added if the plan phase finds it essentially free).
- **Compatibility Claim**: No OPC UA profile/conformance claim changes. The library's public build-system surface (CMake option names, include path, target name) changes in a **breaking** way with this release; there is no published stable release to preserve compatibility with.
- **Application Headroom Goal**: Unchanged — this feature must not measurably change flash/RAM usage (it is a pure rename); the existing `docs/size/feature-size-ledger.md` numbers should still apply after re-running the measurement script under the new build option names.

### Key Entities

- **Library/package identity**: The project's canonical name, appearing as the GitHub repo name, CMake `project()` name, library/target name, umbrella header name, and README title — currently `micro-opcua`/`micro_opcua`, becoming `muc-opcua`/`muc_opcua`.
- **Compile-time option namespace**: Every `MICRO_OPCUA_*` preprocessor/CMake option — becoming `MUC_OPCUA_*`, one-for-one, no option added, removed, or behaviorally changed.
- **Public header tree**: `include/micro_opcua/**` (umbrella `micro_opcua.h` plus per-module headers) — relocated to `include/muc_opcua/**` with umbrella `muc_opcua.h`.
- **Documentation corpus**: README, CLAUDE.md, AGENTS.md, ROADMAP.md, `docs/**` (except the per-feature `docs/traceability/NNN-*.md` files) — living documents updated in place. `specs/001-023` and the per-feature `docs/traceability/NNN-*.md` files are frozen historical record and are not edited at all, the same way past git commits and merged PR descriptions aren't rewritten when a project renames itself.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: A fresh clone of the repository builds successfully (host profile, `ctest` green) using only `MUC_OPCUA_*` CMake options, with zero manual edits required.
- **SC-002**: A full-text search of the repository — excluding `specs/001` through `specs/023`, the per-feature `docs/traceability/NNN-*.md` files (both frozen historical record per FR-006), and any explicit migration/CHANGELOG note that names the old project name for historical/migration purposes — for `micro-opcua`, `micro_opcua`, and `MICRO_OPCUA_` returns zero matches.
- **SC-003**: All CI jobs that were green immediately before this rename (host build, sanitizer build, pico cross-compile, plus best-effort static-analysis/fuzz) are still green immediately after it.
- **SC-004**: 100% of `#include` directives across `src/`, `tests/`, `platform/`, and `examples/` reference `muc_opcua/...` (or the umbrella `muc_opcua.h`), with zero remaining `micro_opcua/...` includes.
- **SC-005**: The measured embedded size ledger (`scripts/measure_size.sh all` or equivalent) reproduces the same nano/micro/embedded/full-featured `.text`/`.data`/`.bss` figures (within normal noise) as the last pre-rename measurement, confirming the rename introduced no code-size regression.
- **SC-006**: An automated check fails the build/test suite if any file outside `specs/001-023/**`, `docs/traceability/NNN-*.md`, and the migration/CHANGELOG note reintroduces the old name strings, so the rename cannot silently regress in a later PR.

## Assumptions

- No published stable release of `micro-opcua` exists yet (confirmed by the requester), so this is treated as a pre-1.0 breaking rename requiring no compatibility shim.
- The GitHub repository rename is already complete (`occamsshavingkit/micro-opcua` -> `occamsshavingkit/muc-opcua`); GitHub's automatic redirect for the old URL is relied on for any external links this migration doesn't catch, not treated as a substitute for updating in-repo references.
- `specs/001` through `specs/023`, and the per-feature `docs/traceability/NNN-*.md` files documenting those same features, are frozen historical record of already-shipped, already-merged work — like past git commits — and this project does not edit them at all, not even to fix a literal old macro name/path/URL reference. The regression guard (FR-008) is scoped accordingly.
- Whether the `mu_`/`opcua_` C identifier prefix changes is explicitly deferred to `/speckit-plan`; this spec's requirements and success criteria assume the prefix is unchanged, and the plan phase will flag it as a decision point if evidence suggests otherwise.
- "Breaking change, no compatibility shim" means old `MICRO_OPCUA_*` macro names and `include/micro_opcua/` paths simply stop existing/working after this ships — there is no dual-support transition period, unless the planning phase finds a near-zero-cost way to add one (e.g. a single deprecated forwarding header), in which case it may be added as a bonus, not a requirement.
