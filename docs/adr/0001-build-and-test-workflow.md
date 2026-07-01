# ADR 0001: Build and Test Workflow

**Date**: 2026-06-25
**Status**: Accepted

## Context

The `muc-opcua` project requires a predictable workflow for host development, embedded cross-compilation, and testing. A standard developer build target summary ensures all developers use reproducible tooling. 

The targets must align with the commands documented in the minimal embedded server quickstart guide.

## Decision

We will standardize on CMake and use specific build configurations for different environments:

1. **Host Build and Test**: `cmake -B build/host -DMUC_OPCUA_BUILD_TESTS=ON` and `ctest`.
2. **Formatting and Static Analysis**: Targets `format-check`, `cppcheck`, and `clang-tidy` generated in the host build.
3. **Sanitizers**: `build/asan` with `-DMUC_OPCUA_SANITIZERS="address,undefined"`.
4. **Fuzzing**: `build/fuzz` with `-DMUC_OPCUA_BUILD_FUZZERS=ON`.
5. **Cross-Compilation (Pico)**: `build/pico` with `-DMUC_OPCUA_PLATFORM=pico`.
6. **Arduino/PlatformIO**: `platformio run -d platform/arduino` for adapter validation.
7. **Traceability/Docs**: Targets `traceability`, `conformance-docs`, and `size-report`.

## Consequences

- **Positive**: CI and local environments use identical workflows.
- **Positive**: Separation of host and embedded builds guarantees portability.
- **Negative**: Developers must maintain multiple CMake build directories.
