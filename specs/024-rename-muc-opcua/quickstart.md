# Quickstart: Verifying the muc-opcua Rename

## Build from a fresh clone (SC-001, SC-004)

```sh
git clone https://github.com/occamsshavingkit/muc-opcua.git
cd muc-opcua
cmake -S . -B build/host -DMUC_OPCUA_BUILD_TESTS=ON -DMUC_OPCUA_BUILD_EXAMPLES=ON -DMUC_OPCUA_PLATFORM=host
cmake --build build/host
ctest --test-dir build/host
```

Expect: configure succeeds using only `MUC_OPCUA_*` option names; build produces
`build/host/src/libmuc_opcua.a`; all tests pass.

## Confirm no old-name includes remain (SC-004)

```sh
grep -rl 'include "micro_opcua/' src/ tests/ platform/ examples/ && echo "FAIL: stale include found" || echo "OK: no stale includes"
```

## Confirm no old-name literals remain anywhere non-historical (SC-002, SC-006)

```sh
grep -rEIl '(micro-opcua|micro_opcua|MICRO_OPCUA|MicroOpcUa|Micro-OPCUA)' . \
  --exclude-dir=build --exclude-dir=build-* --exclude-dir=.git \
  || echo "OK: zero matches"
```

(Once the Phase 2 regression-guard task lands, prefer running that check directly —
this grep is the manual/CI-independent way to spot-check the same invariant.)

## Confirm CI still passes (SC-003)

```sh
# Mirrors .github/workflows/ci.yml's host-build job:
cmake -S . -B build/host -DMUC_OPCUA_BUILD_TESTS=ON -DMUC_OPCUA_BUILD_EXAMPLES=ON \
  -DMUC_OPCUA_PLATFORM=host -DMUC_OPCUA_HAVE_MBEDTLS=ON -DMUC_OPCUA_HAVE_WOLFSSL=ON \
  -DMUC_OPCUA_PUBSUB=ON
cmake --build build/host
ctest --test-dir build/host

# Sanitizer job:
CC=clang CXX=clang++ cmake -S . -B build/asan -DMUC_OPCUA_BUILD_TESTS=ON \
  -DMUC_OPCUA_SANITIZERS="address,undefined" -DMUC_OPCUA_PLATFORM=host \
  -DMUC_OPCUA_HAVE_MBEDTLS=ON -DMUC_OPCUA_HAVE_WOLFSSL=ON -DMUC_OPCUA_PUBSUB=ON
cmake --build build/asan

# Pico cross-compile job (requires PICO_SDK_PATH):
PICO_SDK_PATH=~/pico-sdk cmake -S . -B build/pico -DMUC_OPCUA_PLATFORM=pico \
  -DMUC_OPCUA_BUILD_EXAMPLES=ON -DMUC_OPCUA_PUBSUB=ON
cmake --build build/pico
```

## Re-measure embedded size (SC-005)

```sh
scripts/measure_size.sh all
```

Expect: nano/micro/embedded/full-featured `.text`/`.data`/`.bss` figures match
`docs/size/feature-size-ledger.md`'s last pre-rename entry (within normal
measurement noise) — this is a pure rename, so no code-size delta is expected.

## Confirm the .NET interop harness still resolves (User Story 3 supporting check)

```sh
cd tests/interop/dotnet
dotnet build
```

Expect: build succeeds under the `MucOpcUa.Interop` namespace with no leftover
`MicroOpcUa` reference.
