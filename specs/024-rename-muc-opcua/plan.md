# Implementation Plan: Rename micro-opcua to muc-opcua

**Branch**: `024-rename-muc-opcua` | **Date**: 2026-07-01 | **Spec**: [spec.md](./spec.md)
**Input**: Feature specification from `/specs/024-rename-muc-opcua/spec.md`

## Summary

Rename the project's public identity end to end: `MICRO_OPCUA_*` macros/CMake
options -> `MUC_OPCUA_*`, `include/micro_opcua/` -> `include/muc_opcua/` (incl. the
umbrella header and every internal `#include`), CMake project/library/module
names, the .NET interop harness namespace, and all living documentation/CI/dev
metadata. The GitHub repository rename (`occamsshavingkit/micro-opcua` ->
`occamsshavingkit/muc-opcua`) is already done outside this plan. `mu_`/`opcua_` C
symbol prefixes are explicitly kept unchanged (research.md Decision 3). This is a
pure identifier rename with **zero** OPC UA wire-behavior, StatusCode, or
conformance-claim change, and is expected to be **zero-cost in flash/RAM** — a
successful outcome is measured by "nothing changed except names."

## Technical Context

**Language/Version**: Freestanding C11 core (unchanged); build tooling is CMake 3.x + Bash/Python scripts; the interop harness is C#/.NET 8
**Primary Dependencies**: None new. Existing: CMake, Unity (host tests), OpenSSL/mbedTLS/wolfSSL (optional crypto backends), OPC Foundation UA-.NETStandard reference client (interop only)
**Storage**: N/A (no data storage; this is a build-system/identifier rename)
**Testing**: Existing `ctest` suite (host build) MUST stay green; a new stale-old-name regression check is added (research.md Decision 6 / contracts/regression-guard-contract.md); no new protocol tests needed since no protocol behavior changes
**Target Platform**: Unchanged — RP2040/Cortex-M0+ (Pico SDK), Arduino-compatible, host (dev/test/interop)
**Project Type**: Portable C library with thin platform adapters and examples (unchanged)
**Performance Goals**: N/A — no runtime performance surface touched
**Constraints**: Zero flash/RAM delta expected (pure rename); every currently-green CI job MUST stay green; no OPC UA behavior change permitted
**Scale/Scope**: ~280 files across `include/`, `src/`, `tests/`, `platform/`, `examples/`, `cmake/`, `docs/`, `specs/001-023` (literal-string touch only), `.github/`, `.devcontainer/`, `.specify/`, plus 4 root docs (README/CLAUDE/AGENTS/ROADMAP) and 1 root `optimize-hot-paths.md`
**OPC UA Normative References**: None new — see spec.md OPC-001 through OPC-005 (this feature changes no normative behavior)
**Target OPC UA Profile/Conformance Units**: Unchanged from pre-rename state (Nano/Micro/Embedded 2017, all implementation-complete, profile-**targeting**)
**Conformance Status Target**: profile-targeting (unchanged; this feature must not accidentally alter this claim while editing conformance docs for the name)

## Embedded Size Budget

**Flash Impact**: 0 B expected. This changes identifier text only (macro names, paths, target names); no code logic, no new/removed translation units beyond a straight file move (`include/micro_opcua/*` -> `include/muc_opcua/*`, `cmake/MicroOpcUa*.cmake` -> `cmake/MucOpcUa*.cmake`). Verified by re-running `scripts/measure_size.sh all` post-rename and diffing against `docs/size/feature-size-ledger.md`'s last entry (SC-005).
**RAM Impact**: 0 B expected, same reasoning — no struct layout, no static table, no buffer sizing changes anywhere in this plan.
**Heap Use**: Unchanged (still none in the protocol hot path; this feature doesn't touch that code at all).
**Static Tables Added**: None.
**Transport Buffers**: Unchanged.
**Crypto Dependency Impact**: None — `MICRO_OPCUA_HAVE_{OPENSSL,MBEDTLS,WOLFSSL}` become `MUC_OPCUA_HAVE_{OPENSSL,MBEDTLS,WOLFSSL}` with identical backend behavior.

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

- **Constitution self-reference**: This feature edits `.specify/memory/constitution.md`
  itself (its title string). research.md Decision 8 resolves — explicitly, per
  `/speckit-analyze` finding A2 — that this is a PATCH-level version bump under
  the constitution's own Versioning policy (wording-only correction, no
  principle change), not a full MINOR/MAJOR Amendment, and that the Governance
  section's ceremony requirements are satisfied jointly by T015/T016/T024. This
  is not itself a Constitution Check gate item — it's a scoping note so the
  self-reference isn't mistaken for a bootstrapping problem.
- **Spec Fidelity**: PASS — this feature makes zero wire-visible or conformance
  claims changes; every existing StatusCode/service/encoding/conformance citation
  in `docs/conformance/*` continues to describe identical behavior, just reachable
  through renamed build identifiers.
- **Embedded C Core**: PASS — freestanding C11 core untouched; only macro names,
  include paths, and CMake/doc metadata change. No platform-adapter boundary is
  crossed or altered.
- **Memory Model**: PASS — no heap, buffer, or address-space storage changes of
  any kind; this is a pure identifier rename (see Embedded Size Budget above).
- **Minimal OPC UA Surface**: PASS — no service/transport/encoding surface is
  added, removed, or altered.
- **Profile Research**: PASS (N/A) — no profile/conformance-unit claim changes;
  research.md Decision 2 explicitly confirms the OPC UA Nano/Micro/Embedded
  profile-tier vocabulary (`nano`/`micro`/`embedded`/`full` values, and all
  "Micro [Embedded Device 2017 Server] Profile" prose) is untouched by this
  rename's literal-string substitution set, so no profile claim can be
  accidentally corrupted.
- **Correctness Gates**: PASS — no binary encoder/decoder, chunking, state
  machine, or StatusCode logic is touched; the existing host test suite is the
  correctness gate for "nothing broke," plus the new stale-old-name regression
  guard (FR-008) is itself a new correctness gate, test-first per Constitution
  Principle IV (the guard's failing-on-old-tree check in
  contracts/regression-guard-contract.md's Verification section is written before
  the rename completes, to prove it actually detects what it's meant to detect).
- **Security Honesty**: PASS — no SecurityPolicy, crypto backend, or
  conformance-status wording changes beyond literal identifier renames (e.g.
  `MICRO_OPCUA_SECURITY` -> `MUC_OPCUA_SECURITY` gates exactly the same code as
  before).
- **Toolchain Discipline**: PASS — CMake remains the build system; all existing
  CI jobs (host build, sanitizers, Pico cross-compile, static analysis/fuzz) are
  updated in place to the new option names and MUST stay green (SC-003), not
  replaced or removed.
- **Size Discipline**: PASS — flash/RAM impact is estimated at zero and will be
  measured post-rename against the existing ledger (SC-005); no static table,
  buffer, or crypto dependency is added.

## Project Structure

### Documentation (this feature)

```text
specs/024-rename-muc-opcua/
├── plan.md              # This file
├── research.md          # Phase 0 — literal-string substitution set, profile-tier collision analysis, prefix decision, regression-guard approach
├── data-model.md         # Phase 1 — identifier-class inventory (macros, headers, CMake identity, docs, CI, interop harness, regression guard)
├── quickstart.md          # Phase 1 — build/verify/measure commands post-rename
├── contracts/
│   ├── build-system-contract.md       # Before/after table of every renamed public build-facing identifier
│   └── regression-guard-contract.md   # FR-008 stale-old-name check: inputs, behavior, allow-list resolution
├── checklists/requirements.md
└── tasks.md             # Phase 2 (/speckit-tasks — not created here)
```

### Source Code (repository root)

```text
include/
└── muc_opcua/            # RENAMED from micro_opcua/ — umbrella muc_opcua.h + 13 module headers (incl. services/)
src/
├── core/                 # #include "muc_opcua/..." updated; MICRO_OPCUA_* macro references updated in-place; no logic change
├── encoding/              #   (same treatment across every layer below)
├── services/
├── address_space/
├── security/
├── platform/
└── generated/
cmake/
├── MucOpcUaOptions.cmake        # RENAMED from MicroOpcUaOptions.cmake
├── MucOpcUaCodegen.cmake        # RENAMED from MicroOpcUaCodegen.cmake
├── MucOpcUaStaticAnalysis.cmake # RENAMED from MicroOpcUaStaticAnalysis.cmake
└── MucOpcUaWarnings.cmake       # RENAMED from MicroOpcUaWarnings.cmake
CMakeLists.txt                   # project(muc_opcua ...); MUC_OPCUA_* options; include(cmake/MucOpcUa*.cmake)
src/CMakeLists.txt                # add_library(muc_opcua ...); target_include_directories(muc_opcua ...)
platform/
├── pico/                 # #include updates only
└── arduino/               # #include updates only
examples/
├── minimal_server/        # #include updates only
└── pubsub_server/          # #include updates only
tests/
├── unit/ integration/ fuzz/ benchmark/ support/   # #include updates; new regression-guard test (or tests/tools/ script)
└── interop/dotnet/         # RootNamespace/namespace/URI strings: MicroOpcUa -> MucOpcUa, micro-opcua -> muc-opcua
docs/
├── (all files)             # living docs updated: prose, macro/path examples, size-ledger surrounding text (measured numbers unchanged)
└── traceability/            # this feature's own traceability row added; specs/001-023 traceability docs get literal-string fixes only
specs/001-023/**             # literal-string substitution applied (research.md Decision 4); no narrative rewritten
README.md / ROADMAP.md / CLAUDE.md / AGENTS.md / optimize-hot-paths.md   # updated in place
.specify/memory/constitution.md   # title string only ("micro-opcua Constitution" -> "muc-opcua Constitution"); PATCH version bump (wording correction, no principle change) per its own Governance section
.specify/templates/{plan,tasks}-template.md   # literal-string fixes (these are template scaffolding referenced by every future spec)
.github/workflows/ci.yml          # every -DMICRO_OPCUA_* flag -> -DMUC_OPCUA_*
.devcontainer/devcontainer.json    # "name" field + baked updateContentCommand
```

**Structure Decision**: No new directories beyond the header-tree move
(`include/micro_opcua/` -> `include/muc_opcua/`) and the CMake-module rename
(`cmake/MicroOpcUa*.cmake` -> `cmake/MucOpcUa*.cmake`). Every other change is
in-place content editing of existing files per the five literal-string patterns in
research.md Decision 1, plus one net-new artifact: the stale-old-name regression
check (a new test file under `tests/unit/` or a new script under `tests/tools/` —
task-level choice, see contracts/regression-guard-contract.md) and one migration
note (new or appended to an existing CHANGELOG-equivalent — task-level choice of
exact location, e.g. a new top-level `CHANGELOG.md` entry or a `docs/migration/`
note, to be resolved in `/speckit-tasks`).

## Complexity Tracking

*No Constitution Check violations — table intentionally empty.*

| Violation | Why Needed | Simpler Alternative Rejected Because |
|-----------|------------|-------------------------------------|
| (none) | — | — |
