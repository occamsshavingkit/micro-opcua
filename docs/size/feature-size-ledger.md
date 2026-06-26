# Feature Size Ledger

Measured footprint of the micro-opcua server. The library holds **no mutable
global state** (`.data` and `.bss` are zero in every translation unit), so all
RAM is caller-provided; "Heap" is zero everywhere (no `malloc` in the protocol
path).

- **Measured**: 2026-06-26
- **Toolchain**: `arm-none-eabi-gcc` 13.2.1 (RP2040 / Pico SDK), `gcc` (host)
- **How**: `arm-none-eabi-size` over the `micro_opcua` objects and the example
  ELF; `sizeof(struct mu_server)` from the headers. Reproduce with
  `cmake -DMICRO_OPCUA_PLATFORM=pico -DMICRO_OPCUA_BUILD_EXAMPLES=ON` then
  `arm-none-eabi-size`.

## Summary

| Build | Flash | RAM (static) | RAM (peak stack) | Heap |
|---|---|---|---|---|
| RP2040 — micro-opcua **core library** | 14.5 KiB | 0 B | — | 0 |
| RP2040 — **minimal example** (core + Pico SDK + tinyusb/stdio) | ~41 KiB | ~24.9 KiB | ~5 KiB | 0 |
| Host — minimal example (host libc) | ~46.5 KiB | ~20.5 KiB | ~5 KiB | 0 |

## Breakdown (RP2040)

### Flash
- micro-opcua core (all `src/**/*.c`): **14,841 B (14.5 KiB)** text, 0 data, 0 bss.
- Full example ELF: **41,908 B** text — the remaining ~27 KiB is the Pico SDK
  runtime (tinyusb, stdio_usb, newlib, crt0), which the budget excludes.

### RAM
All caller-provided; the library itself adds 0 static RAM.
- Per-server context `sizeof(struct mu_server)`: **344 B** (config struct = 200 B).
  `MU_SERVER_STORAGE_BYTES` default (1024) is ample; the example over-allocates 4096.
- Default RX + TX transport buffers: **2 × 8192 = 16 KiB** (`MU_MIN_CHUNK_SIZE`).
- **Static RAM for the protocol ≈ 16.3 KiB** (344 B context + 16 KiB buffers).
- Peak stack: a `Read`/`Browse` uses **~4.9 KiB** of stack arrays
  (`MU_DISPATCH_MAX_READ_NODES=32`, browse refs 32). Transient, not static.
- **Total RAM (static + peak stack) ≈ 21 KiB.**

### Heap
- **0 bytes.** No `malloc` in the protocol hot path; storage and buffers are
  caller-owned.

## Budget (specs/001-minimal-embedded-server/plan.md)

| Budget | Target | Measured | Status |
|---|---|---|---|
| Flash (core + example, excl. board TCP/IP) | ≤ 128 KiB | ~15 KiB core / ~41 KiB image | ✅ well under |
| RAM (static + peak stack, excl. platform TCP/IP buffers) | ≤ 32 KiB | ~21 KiB | ✅ under |
| Heap | no mandatory heap | 0 | ✅ |

On the RP2040's 264 KiB RAM this leaves roughly **240 KiB** for the application
and network stack.

## Notes
- The ~4.9 KiB peak stack grew from ~2 KiB when `MU_DISPATCH_MAX_READ_NODES` was
  raised 8 → 32 to accept the OPC Foundation .NET client's batch OperationLimits
  read. Levers if stack-constrained: move the batch arrays into the (static)
  server context, or lower the cap and advertise a real `MaxNodesPerRead`.
- The example's `g_server_storage` is 4096 B but only 344 B are used.
