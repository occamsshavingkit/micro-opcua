# Contract: Public Build-System Surface (Breaking Change)

This is the "public API" this feature changes: everything an external integrator's
build invokes by name. There is no runtime/wire contract change (see spec OPC-001
through OPC-005) — only this build-time naming surface.

## Before -> After

| Surface | Before | After |
|---|---|---|
| Repository | `github.com/occamsshavingkit/micro-opcua` | `github.com/occamsshavingkit/muc-opcua` (already done) |
| CMake project name | `project(micro_opcua ...)` | `project(muc_opcua ...)` |
| CMake library target | `add_library(micro_opcua STATIC ...)` | `add_library(muc_opcua STATIC ...)` |
| Built archive | `libmicro_opcua.a` | `libmuc_opcua.a` |
| Public header umbrella | `#include "micro_opcua/micro_opcua.h"` | `#include "muc_opcua/muc_opcua.h"` |
| Public header tree | `include/micro_opcua/*.h` | `include/muc_opcua/*.h` |
| CMake options (all) | `MICRO_OPCUA_*` (e.g. `MICRO_OPCUA_BUILD_TESTS`) | `MUC_OPCUA_*` (e.g. `MUC_OPCUA_BUILD_TESTS`) |
| CMake option **values** | `MICRO_OPCUA_PROFILE=micro` (unchanged value) | `MUC_OPCUA_PROFILE=micro` (value `micro` still means the OPC UA Micro profile tier — see research.md Decision 2) |
| CMake helper modules | `cmake/MicroOpcUa{Options,Codegen,StaticAnalysis,Warnings}.cmake` | `cmake/MucOpcUa{Options,Codegen,StaticAnalysis,Warnings}.cmake` |
| C preprocessor macros | `MICRO_OPCUA_*` (feature gates, header guards) | `MUC_OPCUA_*` |

## Compatibility

- **No shim.** Old CMake option names, the old include path, and the old macro
  prefix stop existing entirely once this ships (research.md Decision 7). A build
  script that still passes `-DMICRO_OPCUA_BUILD_TESTS=ON` gets an "unused variable"
  CMake warning at best and silently builds with defaults at worst — this is called
  out explicitly in the migration note (FR-007) so it is a documented, intentional
  break, not a silent behavior change.
- **Profile-tier values (`nano`/`micro`/`embedded`/`full`/`custom`) are unaffected**
  by design — only the *option name* `MICRO_OPCUA_PROFILE` becomes
  `MUC_OPCUA_PROFILE`; its accepted values are untouched.
- **No OPC UA wire/behavioral compatibility change of any kind** — every
  conformance-unit claim in `docs/conformance/*.md` continues to describe identical
  runtime behavior.

## Verification

- SC-001: fresh clone + documented CMake invocation (using only `MUC_OPCUA_*`
  options) configures and builds successfully, `ctest` green.
- SC-004: zero `#include "micro_opcua/...".h` remain anywhere in `src/`, `tests/`,
  `platform/`, `examples/`.
- SC-005: `scripts/measure_size.sh all` (or its renamed equivalent) reproduces the
  same nano/micro/embedded/full-featured archive `.text`/`.data`/`.bss` figures
  already recorded in `docs/size/feature-size-ledger.md`.
