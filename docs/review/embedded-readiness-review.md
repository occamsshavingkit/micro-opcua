# Embedded-Readiness Review

A whole-project review of micro-opcua against embedded-systems best practices
(resource-constrained MCU target: RP2040/Pico, Cortex-M0+, no FPU, small stack,
no heap). Conducted 2026-06-26 across four dimensions: memory/stack, code
size/modularity, concurrency/I-O/robustness, and portability/MCU-correctness.

**Overall:** the core is in genuinely good shape for an MCU — no heap, zero
mutable global state (`.data`/`.bss` empty in every core object, verified with
`nm`), endian-explicit byte-shift wire codec, fixed-width types, no unaligned
casts, `-Wconversion -Wcast-align -Werror` enforced, and hosted libc/OS deps
cleanly quarantined into host-only adapters. The findings below are about
hardening and footprint, not a broken design. The single most urgent item is a
stack-depth regression introduced by the (optional) security layer.

Severity is relative to the stated MCU target.

---

## Feature 020 US4 T102b Resource Refresh

Updated 2026-06-30 after the blocker-remediation pass. This section supersedes
the pre-remediation T088/T089/T090 resource blocker status below while retaining
the older review text as historical context.

Current evidence:

- Host/full default build with `MICRO_OPCUA_OPTIMIZE_SIZE=ON` reports
  `text=96,398 B`, `data=6,224 B`, `bss=0 B`, `dec=102,622 B`, which is
  `56,311 B` below the host baseline `text=152,709 B`.
- ARM Cortex-M0+ matrix passes: nano `16,366/0/0/16,366`, micro
  `23,873/0/0/23,873`, embedded `43,078/0/0/43,078`, full-featured
  `51,172/0/0/51,172`.
- Pico embedded build produces `libmicro_opcua.a`, `pico_minimal_server.elf`,
  and `pico_minimal_server.uf2`; the ELF measures `text=73,428`, `data=0`,
  `bss=119,340`, `dec=192,768`.
- Host stack check passes at `3,040 B` and Pico stack check passes at `2,776 B`,
  both below the `10,240 B` threshold.
- Core protocol hot-path heap review finds no allocator calls in `src/core`,
  `src/encoding`, or `src/services`; platform/backend adapter allocation remains
  outside that core claim.
- Caller-provided storage absolutes are recorded, including
  `MU_CONNECTION_BASE_STORAGE_BYTES=1,328 B`; no unbaselined caller-storage delta
  is invented.

Readiness conclusion for the resource rows: the repository-fixable Feature 020
resource blockers are closed by current evidence. This does not provide external
OPC Foundation CTT evidence. T102c separately closes the source-ID ledger gap in
`docs/validation/audit-hardening-triage-ledger.md`.

---

## Feature 020 US4 No-Heap Evidence (T090a)

Updated 2026-06-30 for `020-audit-hardening` User Story 4. The active resource
budget in `specs/020-audit-hardening/plan.md` and
`docs/validation/audit-hardening.md` requires no mandatory heap allocation in
the protocol hot path, no new default static RAM beyond existing
server/session/channel storage, nano/embedded `.text` growth under +4 KiB,
host/full `.text` growth under +8 KiB, and release-gate honesty until those
checks are measured.

Source-level no-heap check run for this review:

```sh
rg -n 'malloc|calloc|realloc|free\(|OPENSSL_malloc' src include tests
```

Result: exit code `0` with 92 textual matches; transcript:
`/tmp/micro-opcua-review/t090a-no-heap-grep.log`.

Classification of the relevant matches:

- Core protocol hot path: no `malloc`, `calloc`, `realloc`,
  direct `free(`, or `OPENSSL_malloc` allocator call was found in `src/core`,
  `src/encoding`, or `src/services`. The `src/core/service_dispatch.c` and
  `src/services/session.*` matches are `mu_session_find_free`, a fixed-slot
  lookup helper name, not heap allocation.
- Host/platform adapters: allocator matches remain in
  `src/platform/host_tcp_adapter.c`, `src/platform/host_crypto_adapter.c`,
  `src/platform/wolfssl_crypto_adapter.c`, and
  `src/platform/mbedtls_crypto_adapter.c`. These are outside the core protocol
  hot-path claim and cover host TCP context allocation, OpenSSL-backed host
  crypto context/certificate allocation, wolfSSL/mbedTLS adapter context
  allocation, and backend cleanup paths.
- Crypto cleanup hooks: `src/security/sym_chunk.c` calls the configured
  `cipher_ctx_free` adapter hook during symmetric-key cleanup; this is adapter
  cleanup, not a core allocator.
- Tests: `tests/unit/test_mbedtls_adapter.c`, `tests/unit/test_wolfssl_adapter.c`,
  and `tests/unit/test_sym_chunk.c` contain cleanup/helper matches and are not
  production protocol hot-path evidence.
- Public headers: no `include/` matches appeared in the transcript.

Review conclusion: current source evidence supports the narrower claim that
Feature 020 has not introduced a mandatory heap allocation in the core protocol
hot path. It does **not** prove that every platform crypto backend is heap-free,
that third-party OpenSSL/mbedTLS/wolfSSL internals avoid allocation, or that the
full embedded readiness/resource release gate passes.

Release-gate honesty: the source-level no-heap review supports the core
protocol hot-path claim. T102b now supplies the missing host/full, ARM, Pico,
and stack resource measurements in `docs/size/feature-size-ledger.md` and
`docs/size/pico-minimal-server.md`. Platform/backend internals remain
adapter-specific and outside the core no-hot-path-heap claim.

---

## Feature 020 US4 Transport-Buffer Evidence (T090b)

Updated 2026-06-30 for `020-audit-hardening` User Story 4. The active
transport-buffer budget in `specs/020-audit-hardening/plan.md` requires the
server to maintain the `MU_MIN_CHUNK_SIZE` 8192-byte lower bound for receive
storage, keep ACK buffer values no larger than the peer-requested limits per
OPC-10000-6 section 7.1.2.4, and make oversize responses deterministic. The
broader resource gate also requires caller-provided memory, no mandatory
protocol hot-path heap, host/full `.text` growth under +8 KiB, nano/embedded
`.text` growth under +4 KiB, and honest blocked evidence when embedded builds
cannot produce measurements.

Transport storage and sizing evidence:

- Public server configuration exposes caller-owned `receive_buffer` and
  `send_buffer` pointers plus their sizes in `include/micro_opcua/server.h`.
  `src/core/server.c` rejects initialization when either buffer is missing or
  smaller than `MU_MIN_CHUNK_SIZE`.
- `include/micro_opcua/config.h` defines `MU_MIN_CHUNK_SIZE` as `8192` and
  keeps `MU_CONNECTION_RX_BUFFER_SIZE` at or above that floor. Historical size
  evidence in `docs/size/feature-size-ledger.md` accounts for RX/TX transport
  buffers as caller-provided memory: `2 x 8192 = 16 KiB` in addition to
  `MU_SERVER_STORAGE_BYTES`.
- `src/core/tcp_connection.c` rejects HEL requests whose receive or send buffer
  values are below the 8192-byte floor and computes ACK receive/send values as
  the minimum of the peer-requested counterpart and the configured local
  buffer. This supports bounded negotiated chunks without advertising a buffer
  larger than the peer asked to use.
- `src/core/message_chunk.c` rejects chunks below the fixed header size, chunks
  whose declared size exceeds the received buffer, invalid message types, abort
  chunks, and non-final chunks in single-chunk mode before service-body
  dispatch. This is the current single-chunk implementation; chunk reassembly
  is not claimed.
- Secured MSG chunks are decrypted in place in the receive buffer, as already
  recorded in this review and in `docs/size/feature-size-ledger.md`; this
  reduces stack scratch use but does not change the need for caller-owned
  receive/transmit storage sized to the negotiated chunk bounds.

Focused checks run for this review:

```sh
cmake --build build/test --target test_tcp_connection test_message_chunk_errors
ctest --test-dir build/test -R '^(test_tcp_connection|test_message_chunk_errors)$' --output-on-failure
```

Result: both commands exited `0`. Transcripts:
`/tmp/micro-opcua-review/t090b-transport-build.log` and
`/tmp/micro-opcua-review/t090b-transport-ctest.log`. `CTest` reported
`2/2` tests passed, `0` failed, total real time `0.00 sec`. The detailed
`build/test/Testing/Temporary/LastTest.log` output recorded
`test_message_chunk_errors` as `8 Tests 0 Failures 0 Ignored` and
`test_tcp_connection` as `4 Tests 0 Failures 0 Ignored`.

Coverage from the focused run:

- `test_tcp_connection` covers normal HEL/ACK negotiation, ACK send-buffer
  capping by configured local and peer-requested bounds, rejection of HEL buffer
  sizes below the 8192-byte minimum, and default buffer sizing.
- `test_message_chunk_errors` covers too-small, too-large, inconsistent-length,
  invalid-message-type, abort-chunk, no-dispatch abort, no-dispatch non-final,
  and invalid-chunk-type cases.

Limitations and gate honesty:

- These are host unit tests from `build/test`; they are focused source/static
  checks for transport buffer and chunk-bound behavior, not Pico or ARM
  Cortex-M0+ resource measurements.
- Current T102b Feature 020 resource evidence supplies host/full, ARM, and Pico
  build artifacts. This T090b entry supports negotiated-buffer behavior and the
  caller-owned-storage model; the resource measurements are recorded in the size
  ledgers.

---

## Feature 020 US4 Static-Storage Evidence (T090c)

Updated 2026-06-30 for `020-audit-hardening` User Story 4. The active static
storage budget in `specs/020-audit-hardening/plan.md` and
`docs/validation/audit-hardening.md` requires no new default static RAM beyond
existing server/session/channel storage unless justified, caller-owned storage
for server state and transport buffers, no mandatory protocol hot-path heap, and
release-gate honesty until host, ARM, and Pico resource evidence all exist.

This T090c entry relies on the current T088 static-storage evidence in
`docs/size/feature-size-ledger.md`; it did not rerun or alter source, tests,
scripts, or task status.

Current host/full archive evidence from T088a/T088c:

```sh
size -t build/src/libmicro_opcua.a
nm -S --size-sort build/src/libmicro_opcua.a | rg ' [BbDd] '
```

Result: both checks are recorded as completed in T088c. `size -t` exited `0`
with GNU `size` 2.42 and reported totals `text=166738`, `data=6224`, `bss=0`,
`dec=172962`; transcript:
`/tmp/micro-opcua-size/t088c-host-full-size-totals.log`. The `nm`/`rg` review
exited `0` with GNU `nm` 2.42; transcript:
`/tmp/micro-opcua-size/t088c-host-full-nm-data-bss.log`.

Static-storage interpretation:

- Archive `.data`: `6,224 B`, flat against the pre-change baseline in
  `docs/size/audit-hardening-baseline.md` (`data +0 B`). The reported initialized
  data is existing archive-owned metadata and tables, including
  `g_supported_services`, server array descriptors/values, `s_base_space`, and
  `s_base_nodes`.
- Archive `.bss`: `0 B`, flat against baseline (`bss +0 B`). The host/full
  symbol review found no `.bss` symbols.
- Caller-provided server storage: current host/full absolute values are
  `sizeof(struct mu_server)=116,040 B` and `MU_SERVER_STORAGE_BYTES=121,516 B`
  under the active full-profile build knobs. These are caller-owned server state,
  not archive `.bss`, but no audit-hardening pre-change baseline exists for these
  values, so no delta or pass claim is made.
- Transport buffers: RX/TX buffers remain caller-owned storage, separately
  reviewed in T090b. They are not counted as archive `.data` or `.bss`.
- Platform/example-owned storage: Pico and example firmware RAM are not measured
  by the host archive checks. `docs/size/pico-minimal-server.md` records that no
  Pico archive, ELF, or UF2 was produced for Feature 020, so there is no current
  Pico `.data`/`.bss` or firmware RAM result.

Embedded/nano status from T088b/T088c:

```sh
BUILD_ROOT=build/audit-size-arm scripts/measure_size.sh all
BUILD_ROOT=build/audit-size-arm scripts/measure_size.sh nano
BUILD_ROOT=build/audit-size-arm scripts/measure_size.sh embedded
find build/audit-size-arm -path '*/src/libmicro_opcua.a' -printf '%p\n'
```

Historical T090c result: the original ARM matrix, nano, and embedded size
attempts exited `2` before current archives were produced. Transcripts:
`/tmp/micro-opcua-size/t088b-arm-size-matrix.log`,
`/tmp/micro-opcua-size/t088b-arm-nano-size.log`,
`/tmp/micro-opcua-size/t088b-arm-embedded-size.log`, and
`/tmp/micro-opcua-size/t088c-audit-arm-archive-find.log`. The archive find
exited `0` but printed no archive paths.

Historical blockers from that pre-remediation run: nano failed on
`src/services/discovery.c:142:29: error: variable 'policy' set but not used
[-Werror=unused-but-set-variable]`; embedded fails
`src/core/server_internal.h:45:1: error: static assertion failed:
"MU_CONNECTION_BASE_STORAGE_BYTES must cover mu_connection_t fields outside
rx_buffer"`.

Reference-only ARM evidence: stale pre-existing archives under `build/size-arm`
reported `data=0`, `bss=0` for nano and embedded, and `nm` found no
`.data`/`.bss` matches. T088c explicitly treats those archives as reference
only because their mtimes predate the current audit-hardening build attempts;
they must not be used as release-gate evidence.

Review conclusion: the historical T090c entry was partial. T102b supersedes it
with current ARM archives, Pico artifacts, flat host archive `.data`/`.bss`, and
explicit caller-storage absolutes in the size ledgers. No unbaselined
caller-storage delta is invented.

---

## Feature 020 US4 Application-Headroom Evidence (T090d)

Updated 2026-06-30 for the T102b blocker-remediation refresh. This review entry
rolls up the current T102b evidence for the application-headroom budget in
`specs/020-audit-hardening/plan.md`, `specs/020-audit-hardening/spec.md`, and
`docs/validation/audit-hardening.md`.

Budget under review:

- Host/full profile `.text` growth must stay under `+8 KiB`.
- Nano and embedded profile `.text` growth must stay under `+4 KiB`.
- The default build must not add static RAM beyond existing
  server/session/channel storage.
- The protocol hot path must not require heap allocation.
- Stack evidence must be recorded for the hardening changes.

Current evidence sources:

- T102b in `docs/size/feature-size-ledger.md` for host/full archive size, ARM
  profile matrix, static archive RAM, caller-storage absolute values, heap-review
  summary, and host/Pico stack results.
- T102b in `docs/size/pico-minimal-server.md` for Pico minimal-server archive,
  ELF, UF2, and stack evidence.
- T090a/T090b/T090c in this review for no-heap, transport-buffer, and
  static-storage interpretation, with historical blockers superseded by T102b.

Application-headroom assessment:

| Budget item | Evidence | Review result | Readiness risk |
|---|---|---|---|
| Host/full `.text` growth under `+8 KiB` | T102b: default full build with `MICRO_OPCUA_OPTIMIZE_SIZE=ON` reports `text=96,398 B`, `data=6,224 B`, `bss=0 B`; pre-change baseline `text=152,709 B`. | **PASS**. Delta is `-56,311 B`, below the `< 8,192 B` growth budget. | Low for the default constrained-device build. |
| Nano `.text` growth under `+4 KiB` | T102b ARM matrix: nano `text=16,366 B`, `data=0`, `bss=0`. | **PASS / current value recorded**. | Low for current measured nano archive. |
| Embedded `.text` growth under `+4 KiB` | T102b ARM matrix: embedded `text=43,078 B`, `data=0`, `bss=0`; Pico embedded artifacts also build. | **PASS / current value recorded**. | Low for current measured embedded archive and Pico build presence. |
| No new default static RAM | Host/full archive `.data=6,224 B`, `.bss=0 B`; ARM archives `.data=0`, `.bss=0`; caller-storage absolutes recorded with `MU_CONNECTION_BASE_STORAGE_BYTES=1,328 B`. | **PASS / absolute caller-storage evidence recorded**. | Low for archive static RAM; caller storage is documented as integrator-provided memory. |
| No protocol hot-path heap | T102b source review found no allocator call in the core protocol hot path; matches are platform/backend adapter cleanup hooks, tests, or `mu_session_find_free` false positives. | **PASS for the core protocol hot path**. | Low for core protocol hot path; adapter internals remain backend-specific. |
| Stack-recording budget | Host stack check `3,040 B`; Pico stack check `2,776 B`; threshold `10,240 B`. | **PASS**. Both measured stack estimates are below threshold. | Low for measured secured OPN stack path. |

Historical build blockers fixed by T102b:

- Nano ARM profile: `src/services/discovery.c:142:29: error: variable 'policy'
  set but not used [-Werror=unused-but-set-variable]`.
- Embedded ARM and Pico profiles:
  `src/core/server_internal.h:45:1: error: static assertion failed:
  "MU_CONNECTION_BASE_STORAGE_BYTES must cover mu_connection_t fields outside
  rx_buffer"`.
- Pico stack check previously failed closed because required `.su` frames were
  missing after the failed build. T102b now records a passing Pico stack check
  with 35 `.su` files and a `2,776 B` secured OPN estimate.

Review conclusion: Feature 020 repository-fixable application-headroom blockers
are closed by current evidence. This does not provide external CTT evidence.
T102c separately closes the source-ID ledger gap in
`docs/validation/audit-hardening-triage-ledger.md`.

---

## HIGH

### H1 — Secured request path peaks at ~25–26 KB of stack — ADDRESSED (in-place sym decrypt)
**Status:** Fixed for the hot MSG path. `mu_sym_chunk_unwrap` now decrypts in place
(frame 8.3 KB → 176 B) and `server.c` no longer uses an 8 KB request scratch
(`handle_data_chunk_secure` 16.4 KB → 6.5 KB). Peak secured-path stack dropped
from ~25 KB to ~12 KB (during dispatch: `respbody[5120]` + the 32-deep dispatch
arrays). Remaining opportunity (follow-up): eliminate `respbody` by framing the
response in place in the send buffer, and apply the same in-place treatment to the
asymmetric OPN buffers. Original finding for reference:


`src/core/server.c:191-192` (`reqscratch[8192]` + `respbody[8192]`),
`src/security/sym_chunk.c` (`buf[MU_SYM_SCRATCH=8192]`),
`src/security/asym_chunk.c` (`plain[2048]` + `sign_buf`/`verify_buf[4096]`),
plus dispatch arrays in `src/core/service_dispatch.c` (`nodes[32]`/`results[32]`,
`ref_pool[32]`).

These nest live on one call chain (`server_poll → process_message →
handle_data_chunk_secure → mu_service_dispatch`/`mu_sym_chunk_unwrap`), summing to
~25 KB before the crypto adapter's own usage. The RP2040 Pico SDK default stack
is 2 KB/core; even a bumped 8 KB blows ~3×. The **plaintext path is fine (~5.5 KB)**
because it writes dispatch output directly into `send_buffer` with no large
locals — so enabling `MICRO_OPCUA_SECURITY` raises peak stack ~5×. Nothing
reserves, documents, or guards this.

Fix (in order of impact):
1. Decrypt **in place**: `mu_sym_chunk_unwrap`'s `buf[8192]` exists only to make
   `[header|plaintext]` contiguous for the HMAC, but the receive buffer already
   holds header-then-ciphertext contiguously; AES-CBC can decrypt in place and
   HMAC can run over it. Removes 8 KB at the deepest point.
2. Drop `reqscratch[8192]` similarly — unwrap into the receive buffer and point
   the reader at it (mirror the plaintext path).
3. Do the same for the asym `sign_buf`/`verify_buf` copies.
4. Move the None-MSG delegation (`server.c:210-213`) above the big locals (or into
   `process_message`) so a None MSG doesn't reserve 16 KB it never uses.
5. Until then: size the scratch from `config.max_message_size`, document a minimum
   stack requirement, and add a `static_assert` budget note.

### H2 — Channel/session lifetime is stored but never enforced (stuck-slot DoS) — ADDRESSED
**Status:** Fixed. `mu_server_poll` now stamps `last_activity_ms` (from
`get_tick_ms`) on accept and on each inbound read, and drops the connection when
idle longer than the channel lifetime (or a 30 s connect timeout before a channel
is open), reclaiming the single slot. A monotonic tick of 0 (stub adapter)
disables the timeout. Covered by `tests/integration/test_connection_robustness.c`.
Original finding:


`src/services/secure_channel.c:48-52`, `src/services/session.c:33-37`,
`src/core/server.c:317-345`. `revised_lifetime`/`revised_session_timeout` are
computed and stored but never read back; `created_at` is only ever set to `0`;
`time_adapter.get_tick_ms` is *required* by config validation yet **never called
anywhere** (verified). With a single client/channel/session slot, a peer that
finishes HELLO/OPN then goes silent (or a half-open TCP peer) holds the only slot
forever — a trivial, permanent single-packet DoS.

Fix: stamp a `last_activity` tick (from `get_tick_ms`) on accept and on each
processed chunk; in `mu_server_poll`, close and reclaim the slot when
`now - last_activity` exceeds the channel lifetime (and a shorter connect-phase
timeout for a pre-OPN connection). Enforce the session timeout likewise.

### H3 — Non-blocking `write()` return and `bytes_written` are ignored — ADDRESSED
**Status:** Fixed. `send_buffer_chunk` (and the HELLO ACK path) now check the
write status and `bytes_written`; on an error or short write the connection is
closed so a half-framed chunk never wedges the slot. Covered by
`test_connection_robustness.c`. Original finding:


`src/core/server.c:113-116` (`send_buffer_chunk`), `:275-277` (HELLO ACK),
`:340-341` (server-full ERR). The TCP adapter is explicitly non-blocking, so
`write()` may legitimately send fewer than `total` bytes; the code discards both
the status and `bytes_written`, silently truncating a framed UASC chunk on the
wire. The client then desyncs on a half-chunk — and with H2 there is no timeout to
recover the wedged slot.

Fix: check the status, and on a short write either retain the unsent tail behind a
send-cursor drained on later polls, or close the connection (reclaiming the slot).

### H4 — No size-oriented build flags (`-Os`, function/data sections, `--gc-sections`) — ADDRESSED
**Status:** Fixed. `cmake/MicroOpcUaCodegen.cmake` compiles the library with
`-ffunction-sections -fdata-sections` and exports `-Wl,--gc-sections` as an
INTERFACE link option, so every executable linking `micro_opcua` strips
unreferenced functions/data at link time (verified: dead `mu_is_unsupported_service`
is absent from the linked example). An opt-in `MICRO_OPCUA_OPTIMIZE_SIZE` adds
`-Os`. Original finding:


`CMakeLists.txt`, `cmake/MicroOpcUaWarnings.cmake`, `platform/arduino/platformio.ini`.
No `-Os`/`-Oz`, no `-ffunction-sections -fdata-sections`, no `-Wl,--gc-sections`,
no default `CMAKE_BUILD_TYPE` (a bare build is `-O0`). Without section GC the
linker keeps whole translation units even if one symbol is referenced, so the
"compile in only what you use" goal isn't realized at link time. (The Pico SDK
enables sections+GC itself, so that path partly escapes; host, the size-report
builds, and the Arduino `platformio.ini` do not.)

Fix: add `-ffunction-sections -fdata-sections` to core compile options and
propagate `-Wl,--gc-sections` to executables; default to a size build type when
unset; optionally an `MICRO_OPCUA_LTO` option. This is the biggest single
footprint lever and unlocks stripping for the items under MED/LOW.

---

## MED

### M1 — Sequence-number validation is implemented but never called — ADDRESSED
**Status:** Fixed. Every inbound OPN/MSG chunk's SequenceNumber is now fed to `mu_sequence_validate` (channel-wide, starting at the OPN); a gap/replay aborts the connection. The validator is reset per connection (not on channel open, which previously skipped the first MSG's check). Covered by `test_server_rejects_sequence_gap`.

_Original:_ ### M1 — Sequence-number validation is implemented but never called (no replay protection)
`src/core/sequence.c` provides `mu_sequence_validate` (gap + wrap handling) and
`mu_secure_channel_t` carries a `sequence` validator that is *initialized* — but
no caller ever invokes it (verified). The inbound SequenceNumber is parsed and
discarded (`server.c`, and the secured path keeps only `request_id`). Result: no
replay or gap/abort detection despite the machinery existing.
Fix: feed the decoded sequence number from each unwrap into
`mu_sequence_validate(&channel.sequence, ...)` and abort the channel on non-GOOD.

### M2 — Services are all-or-nothing — ADDRESSED
**Status:** Fixed. New `MICRO_OPCUA_SERVICE_READ`, `MICRO_OPCUA_SERVICE_BROWSE`, and `MICRO_OPCUA_SERVICE_DISCOVERY` options (default ON) gate the service source files and the matching dispatch-table rows + switch cases, so unselected services compile out entirely (verified: a minimal None-only build without Read/Browse/Discovery is ~22 KB smaller and excludes read.c/browse.c). The OpenSecureChannel/Session services remain always-present; discovery.c's endpoint encoder stays (CreateSession needs it). Original finding:


`src/CMakeLists.txt:11-16`, `src/core/service_dispatch.c` (`g_supported_services[]`
+ dispatch `switch`). Only `MICRO_OPCUA_SECURITY` gates anything; Browse, Read,
Discovery (GetEndpoints/FindServers), and Session are always compiled and the
dispatch table/switch pins them all in even under `--gc-sections`.
Fix: per-service `option(MICRO_OPCUA_SERVICE_BROWSE|READ|DISCOVERY|...)`, gating
both the `target_sources` and the matching rows in `g_supported_services[]` and
the dispatch `switch` so unselected services compile out at the table level.

### M3 — `double`/soft-float in the protocol path — ADDRESSED
**Status:** Fixed for the session path. RequestedSessionTimeout is now read as raw IEEE-754 bits and clamped to [10000, 3600000] ms by integer bit comparison (valid for positive doubles), then echoed via `write_uint64` — no FP compare/convert. Verified: `session.o` has no `__aeabi_d*` symbols on ARM. (ReadRequest maxAge is read with `read_double`, which is a bit-copy, not FP math.) Covered by `test_session` clamp cases. Original finding:


`src/core/service_dispatch.c` (RequestedSessionTimeout), `src/services/session.c:33-37`
(double bounds compares), `src/services/read.c:14` (maxAge). Cortex-M0+ has no FPU,
so these pull in libgcc soft-float (`__aeabi_d*`) for the common connect/read path,
though the logic is pure bounds/compare with no real FP math.
Fix: read the 8 bytes and treat the timeout as integer milliseconds; read-and-ignore
maxAge without `double` arithmetic.

### M4 — No freestanding build is declared or verified — ADDRESSED
**Status:** Fixed. A `freestanding-core` CI job compiles every core source
(everything but the host-only platform adapters, including the security layer)
with `arm-none-eabi-gcc -ffreestanding -Wall -Wextra -Werror`, catching any
hosted-header dependency leaking into the core. Original finding:


CMake never sets `-ffreestanding`/`-nostdlib` and no target/CI compiles the core
without libc; the Pico build links full newlib, and the Arduino skeleton has no
compiled target. The core *is* structurally freestanding (only `<stdint.h>`/
`<stddef.h>`/`<stdbool.h>`/freestanding-safe `<string.h>` in the public surface).
Fix: add a `-ffreestanding -Werror` compile-only sanity target under the
`arm-none-eabi` toolchain and wire it into CI to prevent a hosted-header regression.

### M5 — `mu_server_init` doesn't check caller-storage alignment — ADDRESSED
**Status:** Fixed. `mu_server_init` rejects storage not aligned to `_Alignof(struct mu_server)`; the requirement is documented at the API. Original finding:


`src/core/server.c:87` casts a caller `void *storage` to `mu_server_t *` with no
alignment check; the examples back it with a `uint8_t[]` (declared alignment 1). A
caller using a byte/offset buffer would get a misaligned struct — faulting or
fix-up-slow on Cortex-M0+.
Fix: validate `(uintptr_t)storage % _Alignof(struct mu_server) == 0` in
`mu_server_init`, and/or expose storage as a `union { max_align_t; unsigned char[]; }`;
document the requirement by `MU_SERVER_STORAGE_BYTES`.

### M6 — Inbound SecureChannelId/TokenId never validated — ADDRESSED
**Status:** Fixed. `accept_inbound_chunk` rejects a MSG whose SecureChannelId/TokenId don't match the open channel (both the secure and None paths), aborting the connection.

_Original:_ ### M6 — Inbound SecureChannelId/TokenId never validated
`src/core/server.c`. Unwrap fills `secure_channel_id`/`token_id` but the server
never compares them to the open channel's. For SignAndEncrypt a wrong key fails the
HMAC, but the None-policy MSG path has no channel binding at all.
Fix: reject chunks whose channel id / token id don't match the open channel.

---

## LOW

- **Dead code always linked (absent `--gc-sections`):** `g_unsupported_services[]`
  + `mu_is_unsupported_service()` (`src/core/status.c:45-62`) are never called;
  `src/generated/opcua_ids.c` is an empty TU. Delete or gate.
- **Enum fields cost 4 bytes** in many-instance structs (`mu_variant_t`,
  `mu_datavalue_t`, `mu_node_t`): the 32-deep dispatch result arrays pay 3 wasted
  bytes per enum field. Consider `uint8_t`-backed fields (verify the codec).
- **`mu_status_name()`** (`status.c`) is a ~30-arm debug-string switch; drops out
  under section GC, sits in flash without it.
- **Mock entropy in core:** `src/services/session.c:11-14` fills the nonce with
  `i` instead of the entropy adapter (CreateSession uses a proper `fill_server_nonce`,
  but this path doesn't). Security weakness; replace with the adapter.
- **RSA key-size underflow guard:** `asym_chunk.c` `plain_block = key_bytes - 42`
  underflows for a <336-bit peer cert (caught incidentally downstream). Add an
  explicit `key_bytes <= 42 → Bad_CertificateInvalid`.
- **Negative HELLO MessageSize** accepted (`tcp_connection.c:39-43`, unused after);
  reject `< 8`.
- **Malformed pre-dispatch header** silently dropped with no ServiceFault/close
  (`server.c` `handle_data_chunk_*`); close instead.
- **One poll can run many full crypto cycles** (no `max_messages_per_poll` cap) —
  a long non-yielding burst on a cooperative loop. Add a per-poll cap.
- **Platform skeletons** (`platform/{pico,arduino}/…adapter.c`) stub time/entropy to
  zero — fine as skeletons, but must be replaced before any secure deployment
  (zero nonces).

---

## Suggested remediation order
1. **H1** (stack) — the security layer regressed peak stack ~5×; in-place crypto is the fix.
2. **H3** + **H2** (write handling + timeout) — together they prevent a permanently wedged single slot.
3. **H4** (build flags) — cheap, large footprint win; unlocks dead-code stripping.
4. **M1** (sequence validation) — wire up the code that already exists.
5. **M2** (per-service gating) and the rest as budget allows.
