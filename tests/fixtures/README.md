# Test Fixtures

This directory contains binary fixtures (`*.bin`) and their corresponding generation sidecars.

## Rules
1. Every `*.bin` file MUST have a corresponding Python/bash sidecar script that generated it or validates it.
2. The sidecar script MUST include comments citing the exact OPC UA Part and Section used to derive the binary layout.
3. Tests consuming these fixtures must be registered in CMake.
