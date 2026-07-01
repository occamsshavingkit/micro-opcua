# Phase 1 Data Model: Rename micro-opcua to muc-opcua

This feature has no runtime data model (no new types, no wire format, no OPC UA
entities). The "entities" below are the identifier classes the rename inventories
and tracks to completion — this doubles as the checklist Phase 2 (`/speckit-tasks`)
will turn into concrete file-level tasks.

## Entity: Macro/CMake Option

- **Represents**: A single `MICRO_OPCUA_*` preprocessor macro or CMake
  cache/option variable.
- **Key attributes**: old name (`MICRO_OPCUA_<SUFFIX>`), new name
  (`MUC_OPCUA_<SUFFIX>`), defining file (`CMakeLists.txt` / `src/CMakeLists.txt` /
  `include/micro_opcua/config.h`), consuming files (every `#ifdef`/`#if` site).
- **Validation rule**: Every old name occurrence in a consuming file MUST have a
  matching rename; no macro is renamed in its definition but left referenced under
  the old name anywhere (this would silently disable the feature it gates, since
  `#ifdef MICRO_OPCUA_SECURITY` guarding code that now only ever defines
  `MUC_OPCUA_SECURITY` would never trigger).
- **Known instances** (non-exhaustive, ~30+ distinct suffixes; full enumeration is
  a Phase 2 task): `MICRO_OPCUA_BUILD_TESTS`, `MICRO_OPCUA_BUILD_EXAMPLES`,
  `MICRO_OPCUA_BUILD_FUZZERS`, `MICRO_OPCUA_BUILD_BENCHMARKS`,
  `MICRO_OPCUA_PROFILE` (see Decision 2 in research.md — the *value* `micro` is
  untouched), `MICRO_OPCUA_SECURITY`, `MICRO_OPCUA_SUBSCRIPTIONS`,
  `MICRO_OPCUA_SUBSCRIPTIONS_STANDARD`, `MICRO_OPCUA_SERVICE_{READ,WRITE,BROWSE,
  DISCOVERY,REGISTER_NODES,NODEMANAGEMENT,QUERY,HISTORY,ALARMS_CONDITIONS}`,
  `MICRO_OPCUA_USER_AUTH`, `MICRO_OPCUA_BASE_NODES`, `MICRO_OPCUA_BASE_TYPE_SYSTEM`,
  `MICRO_OPCUA_EVENTS`, `MICRO_OPCUA_PUBSUB`, `MICRO_OPCUA_CUSTOM_METHODS`,
  `MICRO_OPCUA_MULTIPLE_CONNECTIONS`, `MICRO_OPCUA_STATUS_STRINGS`,
  `MICRO_OPCUA_OPTIMIZE_SIZE`, `MICRO_OPCUA_LTO`, `MICRO_OPCUA_SANITIZERS`,
  `MICRO_OPCUA_PLATFORM`, `MICRO_OPCUA_HAVE_{OPENSSL,MBEDTLS,WOLFSSL}`, plus every
  header-guard macro of the form `MICRO_OPCUA_<HEADER>_H`.

## Entity: Public Header / Include Path

- **Represents**: A file under `include/micro_opcua/**`, including the umbrella
  header, and every `#include "micro_opcua/...".h` directive that references it.
- **Key attributes**: old path, new path (`include/muc_opcua/**`), umbrella
  rename (`micro_opcua.h` -> `muc_opcua.h`), list of includer files.
- **Validation rule**: A path move and every includer update land in the same
  atomic change (a build cannot exist where the directory is renamed but includers
  still say `micro_opcua/...` — that's a hard compile failure, not a silent
  regression, so this is self-checking via `ctest`/build success, unlike the macro
  case above).
- **Known instances**: 14 headers per the current tree (`address_space.h`,
  `config.h`, `encoding.h`, `micro_opcua.h` [umbrella], `opcua_ids.h`,
  `opcua_types.h`, `platform.h`, `pubsub.h`, `security.h`, `server.h`,
  `services/alarms_conditions.h`, `services/history.h`, `status.h`, `types.h`).

## Entity: CMake Project/Target/Package Identity

- **Represents**: The library's build-system name as CMake sees it.
- **Key attributes**: `project(micro_opcua ...)` (top-level `CMakeLists.txt`),
  `add_library(micro_opcua STATIC ...)` and its
  `target_include_directories(micro_opcua PUBLIC ...)` (`src/CMakeLists.txt`), and
  the four `include(cmake/MicroOpcUa*.cmake)` module-load lines.
- **Validation rule**: Renamed consistently so `find_package`/`target_link_libraries`
  documentation examples and the actual build artifact name
  (`libmuc_opcua.a`) agree.
- **Known instances**: `cmake/MicroOpcUaOptions.cmake`,
  `cmake/MicroOpcUaCodegen.cmake`, `cmake/MicroOpcUaStaticAnalysis.cmake`,
  `cmake/MicroOpcUaWarnings.cmake` (filenames + their `include()` sites).

## Entity: Documentation Reference

- **Represents**: A prose or code-sample mention of the project name, macro
  name, include path, or repository URL in a Markdown/text file.
- **Key attributes**: file path, whether it is "living" (README, CLAUDE.md,
  AGENTS.md, ROADMAP.md, everything under `docs/`, `.specify/memory/constitution.md`,
  `.specify/templates/*.md`, root-level `optimize-hot-paths.md`) or "historical"
  (`specs/001-*` through `specs/023-*` — per Decision 4 in research.md, still gets
  the literal-string substitution, but its surrounding narrative/reasoning text is
  never rewritten).
- **Validation rule**: Zero remaining occurrences of any Decision-1 literal string
  post-rename, verified by the Decision-6 regression guard.
- **Known instances**: ~280 files matched the literal-string grep (see
  research.md Decision 5); the majority are source/test files where the only hit is
  an `#include` line or header-guard macro, already covered by the Macro/Header
  entities above — the *documentation-specific* set requiring editorial judgment
  (not just search-replace) is: `README.md`, `ROADMAP.md`, `CLAUDE.md`, `AGENTS.md`,
  `.specify/memory/constitution.md`, everything under `docs/`, `optimize-hot-paths.md`.

## Entity: CI/Dev-Environment Reference

- **Represents**: Non-Markdown, non-source configuration referencing the old
  names.
- **Key attributes**: file, field.
- **Known instances**: `.github/workflows/ci.yml` (every `-DMICRO_OPCUA_*` flag
  across all jobs), `.devcontainer/devcontainer.json` (`"name": "micro-opcua"` and
  the baked `updateContentCommand`).
- **Validation rule**: CI jobs green post-rename (SC-003); devcontainer name
  matches the renamed repo for Codespaces UI consistency.

## Entity: .NET Interop Harness Identity

- **Represents**: `tests/interop/dotnet/` project/namespace/URI identifiers.
- **Key attributes**: `RootNamespace` (csproj), C# `namespace` declaration,
  `ApplicationName`/`ApplicationUri`/certificate `SubjectName` string literals in
  `Program.cs`.
- **Validation rule**: `dotnet build`/the interop CI job still succeeds and
  connects, proving the rename didn't silently break the reference-client
  interop check that guards every security-policy change in this repo.

## Entity: Regression Guard (new)

- **Represents**: The FR-008/SC-006 automated check preventing old-name
  reintroduction.
- **Key attributes**: scanned file set (everything tracked, minus
  `build*/`/`.git/`), forbidden-literal set (the five Decision-1 strings, checked
  even inside `specs/001-023` per Decision 4), allow-list (none — see Decision 6:
  after the rename there should be zero remaining old-literal occurrences anywhere
  in the tracked tree, including the migration note itself, which describes the
  change using prose like "previously named MICRO_OPCUA_SECURITY" — Phase 2 tasks
  must decide whether the migration note is itself excluded from the scan or
  phrases the old name in a way the scanner tolerates, e.g. a code-fenced example
  vs. an escaped/split string).
- **Validation rule**: Fails the test/CI run on any forbidden-literal match
  outside the resolved allow-list.
