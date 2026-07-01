# Migration: micro-opcua renamed to muc-opcua

**Feature**: [024-rename-muc-opcua](../specs/024-rename-muc-opcua/) | **Date**: 2026-07-01

## What changed

The project was renamed. This is a **breaking change** to the public
build-system surface, with **no compatibility shim**. There is no published
stable release of the prior name, so nothing older is preserved.

| Surface | Before | After |
|---|---|---|
| GitHub repository | `occamsshavingkit/micro-opcua` | `occamsshavingkit/muc-opcua` |
| CMake project name | `project(micro_opcua ...)` | `project(muc_opcua ...)` |
| CMake library target | `add_library(micro_opcua ...)` | `add_library(muc_opcua ...)` |
| Built archive | `libmicro_opcua.a` | `libmuc_opcua.a` |
| Public header umbrella | `#include "micro_opcua/micro_opcua.h"` | `#include "muc_opcua/muc_opcua.h"` |
| Public header tree | `include/micro_opcua/*.h` | `include/muc_opcua/*.h` |
| Every CMake/preprocessor option | `MICRO_OPCUA_*` (e.g. `MICRO_OPCUA_BUILD_TESTS`, `MICRO_OPCUA_SECURITY`, `MICRO_OPCUA_PROFILE`) | `MUC_OPCUA_*` (e.g. `MUC_OPCUA_BUILD_TESTS`, `MUC_OPCUA_SECURITY`, `MUC_OPCUA_PROFILE`) |
| CMake helper modules | `cmake/MicroOpcUaOptions.cmake` (+ Codegen, StaticAnalysis, Warnings) | `cmake/MucOpcUaOptions.cmake` (+ Codegen, StaticAnalysis, Warnings) |
| .NET interop harness namespace | `MicroOpcUa.Interop` | `MucOpcUa.Interop` |

## What did NOT change

- **The `mu_`/`opcua_` C function and type prefixes** — `mu_server_init`,
  `mu_nodeid_t`, `opcua_statuscode_t`, and every other public/internal C
  symbol keep their existing spelling. `mu_` already reads as the start of
  "muc"; there was no technical reason to also churn every call site in the
  codebase for a rename that was about branding/build-system identity, not
  the C API shape.
- **The `MUC_OPCUA_PROFILE` option's accepted values** — `nano`, `micro`,
  `embedded`, `full`, and `custom` are unchanged. `MUC_OPCUA_PROFILE=micro`
  still means "target the OPC UA **Micro** server profile," exactly as
  `MICRO_OPCUA_PROFILE=micro` did before. The `make micro` build preset and
  the OPC Foundation's own "Micro Embedded Device 2017 Server Profile"
  terminology throughout the conformance docs are untouched — this rename
  exists specifically to stop conflating the *project's* old name with that
  *profile* name, and the profile name itself was never the problem.
- **Any OPC UA protocol behavior, StatusCode, service, encoding, or
  conformance claim.** This is a pure identifier/build-system rename. Target
  status remains **profile-targeting** (no profile-compliant/CTT-verified
  claim), unchanged from before this feature.
- **Historical planning record.** Every `specs/NNN-*/` directory (001 through
  023, and this feature's own 024) and the per-feature
  `docs/traceability/NNN-*.md` files are left completely untouched, including
  wherever they happen to quote the old build option names or include paths.
  They are frozen history, the same way a project rename doesn't rewrite old
  git commits or already-merged pull request descriptions.

## Upgrading an existing integration

There is no dual-support transition period. To update an existing build
against this project:

1. Change your clone/fetch URL to `https://github.com/occamsshavingkit/muc-opcua.git`
   (the old URL redirects automatically via GitHub, but update it anyway).
2. Rename every `-DMICRO_OPCUA_*=...` CMake flag you pass to `-DMUC_OPCUA_*=...`
   (the option names changed one-for-one; no option was added, removed, or
   changed in meaning).
3. Change every `#include "micro_opcua/...".h` to `#include "muc_opcua/...".h"`,
   including the umbrella header (`micro_opcua.h` -> `muc_opcua.h`).
4. If you link against the built library by name (e.g. `target_link_libraries(... micro_opcua)`
   or `-lmicro_opcua`), update it to `muc_opcua`.
5. Your own C code that calls the library's public API (`mu_server_init`,
   `mu_nodeid_t`, etc.) requires **no changes** — those symbols are unchanged.

## Why

The old name conflated three unrelated things: the project's own identity,
the maintainer's location (Munich, airport code MUC), and the OPC UA
Foundation's own "Micro" server profile tier that this library also
implements. `muc-opcua` (µc = "mu-c", for microcontroller) resolves all three
at once without colliding with the OPC UA profile vocabulary. See
[specs/024-rename-muc-opcua/spec.md](../specs/024-rename-muc-opcua/spec.md)
for the full rationale.
