# Implementation Plan: Embedded Profile Completion

**Branch**: `005-embedded-profile-completion` | **Date**: 2026-06-27 | **Spec**: [spec.md](./spec.md)
**Input**: Feature specification from `/specs/005-embedded-profile-completion/spec.md`

## Summary

Raise the `embedded` build from "Micro + Basic256Sha256 (preliminary)" to a conformant,
CTT-targeting **Embedded 2017 UA Server Profile** (OPC-10000-7 §6.6.69). Authoritative
membership is pinned in [research.md](./research.md). Four slices, in priority order:

- **US1 (P1)** — the **Standard DataChange Subscription 2017 Server Facet** delta over the
  Micro tier's Embedded DataChange facet: absolute-deadband DataChangeFilter (OPC-10000-4
  §7.22.2), monitored-item queue depth ≥ 2 with discardOldest + overflow (§5.13.2, §7.20.1),
  SetTriggering (§5.13.5), and raised capacities (≥ 10/≥ 100 monitored items, ≥ 2 subscriptions,
  ≥ 5 parallel Publish).
- **US2 (P2)** — the **Base Info Type System** CU: expose the namespace-0 DataType/ReferenceType/
  ObjectType/VariableType nodes the server uses, with HasSubtype and resolvable HasTypeDefinition
  (OPC-10000-5; OPC-10000-3), plus ServerProfileArray advertising the profile.
- **US3 (P3)** — a minimal **Call** service (OPC-10000-4 §5.11) implementing the
  **GetMonitoredItems** and **ResendData** Server methods (OPC-10000-5), required by the §6.6.17
  "2017" facet; all other methods rejected with cited StatusCodes.
- **US4 (P4)** — build wiring, profile isolation, and conformance + size documentation at
  `profile-targeting`.

**Scope corrections from research** (override the first draft): TransferSubscriptions is OUT
(Client Redundancy Facet, not required); SetTriggering is IN; percent deadband is OUT (Data
Access Facet); the minimal Method surface is IN (user-confirmed full-2017 conformance). All work
preserves freestanding C11, no-heap, no-mutable-globals, bounded execution, and the size ledger.

## Technical Context

**Language/Version**: Freestanding C11 (C99-compatible subset where practical)  
**Primary Dependencies**: None in the core; pluggable crypto adapter for the already-shipped Basic256Sha256 only  
**Storage**: No heap. Fixed-size, caller-provided `struct mu_server` storage; static const address-space tables  
**Testing**: Host unit tests (`tests/`), sanitizer (ASan/UBSan) build, fuzz targets for new decoders, asyncua + .NET reference-client interop in CI  
**Target Platform**: RP2040 / Cortex-M0+ (no FPU) primary; Arduino-compatible; host (dev/test)  
**Project Type**: Portable C library with thin platform adapters and examples  
**Performance Goals**: Bounded per-poll execution; no dynamic allocation; sampling/publish driven cooperatively by `mu_server_poll`  
**Constraints**: Core flash ≤ 128 KiB (excl. board TCP/IP + crypto backend); RAM ≤ 32 KiB static + peak stack (excl. transport buffers) — *note: the mandated ≥ 100-item capacity raises caller-provided `struct mu_server` storage materially (see Size Budget)*; `.bss` = 0; no mutable globals; no mandatory heap  
**Scale/Scope**: Single TCP connection multiplexing up to `MU_MAX_SESSIONS` (default 2) sessions; `embedded` raises `MU_MAX_MONITORED_ITEMS`≥100, `MU_MAX_SUBSCRIPTIONS`≥2, `MU_MAX_PUBLISH_REQUESTS`≥5, `MU_MONITORED_QUEUE_DEPTH`≥2  
**OPC UA Normative References**: OPC-10000-4 §5.11 (Call), §5.13.2 (CreateMonitoredItems/MonitoringParameters), §5.13.5 (SetTriggering), §5.14.2/.5/.6 (Subscription minimums/Publish/Republish), §7.20 + §7.20.1 (NotificationMessage/overflow), §7.22.2 (DataChangeFilter/DeadbandType); OPC-10000-5 (standard NodeSet: type system, GetMonitoredItems/ResendData methods, Server object); OPC-10000-3 (address space model); OPC-10000-6 (UA Binary, UA-TCP, status-code encoding); OPC-10000-7 §6.6.69 (Embedded 2017 profile), §6.6.17 (Standard DataChange 2017 facet), §4.2 (CUs/Groups)  
**Target OPC UA Profile/Conformance Units**: Embedded 2017 UA Server Profile = Micro 2017 + Basic256Sha256 + Standard DataChange Subscription 2017 Server Facet + Base Info Type System (full membership: research.md)  
**Conformance Status Target**: `profile-targeting` (CTT run is an external step)

## Embedded Size Budget

**Flash Impact**: Estimated **+3–5 KiB** core `.text` over today's ~27.1 KiB embedded core:
absolute-deadband comparison incl. soft-float for Float/Double (~0.3–0.6 KiB), per-item queue
+ overflow logic (~0.3–0.6 KiB), SetTriggering handler (~0.3–0.5 KiB), Call service +
GetMonitoredItems/ResendData (~0.8–1.5 KiB), enlarged type-system const node set (~1.0–2.0 KiB).
Measured per US into `docs/size/feature-size-ledger.md`.  
**RAM Impact (significant, mandated)**: raising `MU_MAX_MONITORED_ITEMS` from 8 to ≥ 100 grows
the fixed MonitoredItem array in `struct mu_server` by ~92 × `sizeof(mu_monitored_item_t)`
(≈ **+11–15 KiB**), plus queue depth ≥ 2 per item, plus ≥ 5 parked Publish slots. All
caller-provided; `MU_SERVER_STORAGE_BYTES` for `embedded` recomputed (expected ~25–30 KiB) and
documented. Still no static RAM in the library; `.bss` stays 0. On RP2040 (264 KiB) this leaves
ample headroom; the `Monitor Items 100` CU makes it non-optional. `micro`/`nano` keep their small
capacities (their facet needs only `Monitor Items 2`).  
**Heap Use**: None. Fixed-size arrays only.  
**Static Tables Added**: Enlarged const Base Information type-system node set + method nodes in
`src/address_space/base_nodes.c` (and `generated/` if a compact table is warranted).  
**Transport Buffers**: Unchanged (2 × 8 KiB caller-owned). `MU_RETRANSMIT_BYTES` (256 B/sub)
reviewed against Standard-facet NotificationMessage sizes (§5.14.6).  
**Crypto Dependency Impact**: Unchanged (Basic256Sha256 already shipped). The soft-float from
absolute-deadband on Float/Double is libgcc runtime, not crypto (Complexity Tracking).

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

- **Spec Fidelity**: PASS — every added service/filter/method/StatusCode cites exact OPC-10000
  sections; profile/facet/CU membership pinned in `research.md` from the authoritative OPC
  Foundation API before any compliance claim; over-capacity/unsupported conditions return cited
  StatusCodes.
- **Embedded C Core**: PASS — freestanding C11; new logic in the portable core
  (`services/subscription.c`, a small Call handler, `address_space/base_nodes.c`); platform stays
  behind adapters.
- **Memory Model**: PASS — no heap; raised capacities and per-item queues are fixed-size in
  caller-provided storage; type nodes are `const`.
- **Minimal OPC UA Surface**: PASS *with justified additions* — the Call service is normally out
  of scope but is **required** by the §6.6.17 facet (user-confirmed full-2017 target); it is
  limited to two methods. Documented in Complexity Tracking.
- **Profile Research**: PASS — `research.md` justifies the exact target profile and facet/CU set
  from OPC-10000-7 / the Profile Reporting API.
- **Correctness Gates**: PASS — test-first: failing host tests + fixtures for deadband, queueing,
  SetTriggering, Call/GetMonitoredItems/ResendData, type-node browse, and malformed-input
  rejection precede implementation; fuzz extended to new decoders; sanitizer build clean.
- **Security Honesty**: PASS — no new security policy; None gated; Basic256Sha256 unchanged and
  pluggable; status stays `profile-targeting` until CTT.
- **Toolchain Discipline**: PASS — CMake/Make profile wiring, host tests, ASan, formatting, static
  analysis, and the Pico cross-compile all gate each PR.
- **Size Discipline**: PASS *with documented mandated growth* — flash/RAM impact estimated above
  and measured per US into the size ledger; the ≥ 100-item RAM growth and Call-service flash are
  required by the facet and recorded, not hidden.

**Post-Phase-1 re-check**: design (data-model.md, contracts/) keeps no-heap/no-globals and adds
no surface beyond the cited CUs — Constitution Check still PASS. Re-confirmed after Phase 1.

## Project Structure

### Documentation (this feature)

```text
specs/005-embedded-profile-completion/
├── plan.md              # This file
├── research.md          # Phase 0 — authoritative profile/facet/CU rollup + decisions
├── data-model.md        # Phase 1 — extended subscription/method/type entities
├── quickstart.md        # Phase 1 — build/test/measure/interop + acceptance mapping
├── contracts/           # Phase 1 — wire/behaviour contracts
│   ├── datachange-deadband-filter.md
│   ├── monitoreditem-queue.md
│   ├── set-triggering.md
│   ├── method-call.md
│   └── information-model-exposure.md
├── checklists/requirements.md
└── tasks.md             # Phase 2 (/speckit-tasks — not created here)
```

### Source Code (repository root)

```text
include/micro_opcua/        # config.h capacity macros (queue depth, raised maxima), server.h
src/
├── core/
│   ├── service_dispatch.c  # US1: add SetTriggering; US3: add Call — to the dispatch table (gated)
│   └── server.c            # poll/tick wiring (ResendData re-report flag handled in tick)
├── services/
│   ├── subscription.c      # US1: deadband, queue≥2/discardOldest/overflow, SetTriggering, capacities; US3: GetMonitoredItems/ResendData engine support
│   └── subscription.h      # US1: extend mu_monitored_item_t (deadband, queue ring, trigger links); new prototypes
├── address_space/
│   ├── base_nodes.c        # US2: namespace-0 type-system nodes + HasTypeDefinition + ServerProfileArray; US3: GetMonitoredItems/ResendData method nodes + Input/OutputArguments
│   └── base_nodes.h        # US2/US3: accessors
├── encoding/               # US1: decode DataChangeFilter deadband, SetTriggering; US3: decode Call request / encode Call response
└── generated/              # US2: optional compact const type-node table
tests/{unit,fixtures,fuzz}/ # US1–US3 test-first suites + byte fixtures + decoder fuzz
docs/conformance/           # US4: profile-embedded.md + status.md update
docs/size/                  # US4: feature-size-ledger.md refresh
Makefile / CMakeLists.txt   # US4: embedded `-D` capacity overrides; confirm wiring; nano/micro unchanged
```

**Structure Decision**: Reuse the existing layout. US1 extends the no-heap subscription engine in
place and adds a SetTriggering dispatch entry. US3 adds a small Call handler (a new dispatch entry
+ a two-method switch) and the method nodes in `base_nodes.c`. US2 grows the static address space
in `base_nodes.c` (const structural nodes via the established pattern). US4 is build wiring and
docs. No new directories/modules.

## Complexity Tracking

| Violation | Why Needed | Simpler Alternative Rejected Because |
|-----------|------------|-------------------------------------|
| Call service set (two methods) added to an otherwise method-free server | The Standard DataChange Subscription **2017** facet (§6.6.17) makes `Method Call`, `GetMonitoredItems`, and `ResendData` **required**; user confirmed full-2017 conformance | Omitting Call fails the chosen profile's CUs. Surface is minimized: only GetMonitoredItems/ResendData are dispatched; every other method returns a cited Bad StatusCode. No general user-method framework. |
| `embedded` `MU_MAX_MONITORED_ITEMS` ≥ 100 → ~+11–15 KiB caller storage | `Monitor Items 100` CU requires ≥ 100 monitored items | 8 items (Micro default) fails the CU. Kept fixed-size/no-heap and `-D`-overridable; applied only to the `embedded` build so micro/nano are unaffected; RAM cost measured into the ledger; well within RP2040 264 KiB. A heap pool was rejected (violates memory model). |
| Soft-float comparison on Float/Double for absolute deadband | §7.22.2 absolute Deadband applies to numeric values incl. Float/Double; required by the facet | Integer-only math cannot evaluate `|new−last| ≥ deadband` for floats; refusing float deadband fails the CU. Cost is bounded libgcc soft-float, linked only when exercised; integer types use integer comparison. |
| Per-MonitoredItem notification queue (depth ≥ 2) raising `sizeof(mu_monitored_item_t)` | §5.13.2/§7.20.1 queueSize ≥ 2 + discardOldest + overflow is the `Monitor MinQueueSize_02` CU | Single-slot (queueSize=1) is the Embedded facet only. Kept fixed-size (`MU_MONITORED_QUEUE_DEPTH`, `-D`-overridable); RAM documented. |
| Enlarged static information-model table (`Base Info Type System`) | Mandatory CU at the Embedded tier; broader than Nano's `Base Info Core Structure` | A minimal node set fails the CU's browse/type-resolution. Kept `const` (flash, not RAM) behind `MICRO_OPCUA_BASE_NODES`/profile scope; nano/micro footprint unaffected; flash measured. Bounded to "types the server uses" (research.md), not the whole Part 5 NodeSet. |

*All deviations are required by the target profile's facets (cited), kept fixed-size/no-heap,
gated/`-D`-scoped so lower profiles are unaffected, and measured into the size ledger —
consistent with Size Discipline (reductions must not remove required conformance behavior).*
