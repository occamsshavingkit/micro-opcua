# Phase 0 Research: Embedded Profile Completion

Authoritative determination of the target profile's facet/conformance-unit membership.
Sources are the **OPC Foundation's own systems**: the live Profile Reporting REST API
(`https://profiles.opcfoundation.org/api/`, the JSON backend behind the SPA) and the
OPC-10000-7 v1.04 rendered facet/profile tables (via the Internet Archive of
`reference.opcfoundation.org/v104/Core/docs/Part7/`), cross-referenced with OPC-10000-4
service sections. The reference MCP does not index the profile→facet rollup; this research
closes that gap (the spec's Phase-0 obligation).

## Decision 1 — Target profile identity

- **Decision**: Target the **Embedded 2017 UA Server Profile**,
  `http://opcfoundation.org/UA-Profile/Server/EmbeddedUA2017` (OPC-10000-7 v1.04 §6.6.69),
  building on the Micro Embedded Device 2017 Server Profile the codebase already targets.
- **Rationale**: Canonical name is "Embedded UA Server Profile" (not "Embedded Device
  Server"). The codebase targets the **2017** family (`MicroEmbeddedDevice2017`) and ships
  Basic256Sha256, which is the 2017 generation's required SecurityPolicy. The Embedded UA
  profile *"builds upon the Micro Embedded Device Server Profile. The most important
  additions are: support for security ... and support for the Standard DataChange
  Subscription Server Facet."* (live API id 932, verbatim.)
- **Alternatives considered**: The original Embedded UA Server Profile (§6.6.68) requires
  Basic128Rsa15 — rejected (deprecated; codebase ships Basic256Sha256). The current
  *consolidated* v1.05 tool merges the "2017" profiles back into base names (live tool only
  exposes `EmbeddedUA`/`MicroEmbeddedDevice`/`NanoEmbeddedDevice`); we cite the v1.04 "2017"
  URIs to match the codebase, which is internally consistent with Basic256Sha256.

## Decision 2 — Embedded UA Server Profile composition (what to add over Micro)

Per OPC-10000-7 §6.6.68/§6.6.69 membership tables, the Embedded profile = Micro + these
required components:

| Component | Status | Mapped work |
|---|---|---|
| Micro Embedded Device 2017 Server Profile | **already shipped** | Nano + Embedded DataChange facet + 2 parallel sessions |
| SecurityPolicy – Basic256Sha256 (2017 variant) | **already shipped** | `MICRO_OPCUA_SECURITY` |
| Security Default ApplicationInstance Certificate | already shipped (cert handling in security stack) | verify/document in US (conformance) |
| **Standard DataChange Subscription Server Facet** | **to implement (US1)** | see Decision 3 |
| **Base Info Type System** | **to implement (US2)** | see Decision 4 |

## Decision 3 — Standard DataChange Subscription (2017) facet: the delta to implement

The Micro tier already ships the **Embedded DataChange Subscription Server Facet** (§6.6.15):
CUs `Monitor Basic`, `Monitor Items 2`, `Monitor QueueSize_1`, `Monitor Value Change`,
`Subscription Basic`, `Subscription Minimum 1`, `Subscription Publish Discard Policy`,
`Subscription Publish Min 02`.

The **Standard DataChange Subscription 2017 Server Facet** (§6.6.17) adds the following
**required** CUs (this is the authoritative US1 + US3 scope):

| CU added | Meaning | OPC-10000-4 § |
|---|---|---|
| `Monitor Items 10` | ≥10 monitored items per subscription | §5.13.2 |
| `Monitor Items 100` | ≥100 monitored items (≥½ of required subs, ≥½ sessions) | §5.13.2 |
| `Monitor MinQueueSize_02` | monitored-item queue ≥2 entries (≥⅓ of items) | §5.13.2 (MonitoringParameters: queueSize, discardOldest) |
| `Monitor Triggering` | **SetTriggering** service | §5.13.5 |
| `Monitored Items Deadband Filter` | **absolute** Deadband DataChangeFilter (numeric) | §7.22.2 (DeadbandType = Absolute(1)) |
| `Subscription Minimum 02` | ≥2 subscriptions per session (≥½ sessions) | §5.14.2 |
| `Subscription Publish Min 05` | ≥5 parallel Publish per session + Republish retransmit queue | §5.14.5 / §5.14.6 |
| `Base Info GetMonitoredItems Method` | **GetMonitoredItems** method on the Server object | method behavior via Call §5.12.2.2; method defined OPC-10000-5 §9.1 |
| `Method Call` | **Call** service set | §5.12.2.2 |
| `Base Info ResendData Method` | **ResendData** method on the Server object | method behavior via Call §5.12.2.2; method defined OPC-10000-5 §9.2 |

> **Scope decision (user-confirmed 2026-06-27): Full Embedded 2017, including the minimal
> Method surface.** The §6.6.17 "2017" facet makes `Method Call`, `GetMonitoredItems`, and
> `ResendData` **required** (in the non-2017 §6.6.16 facet they are optional, and the current
> consolidated live profile lists them optional). The user chose full 2017 conformance, so
> these become in-scope (a dedicated Method-service user story, US3).

### CORRECTIONS to the draft spec/plan (this research overrides them)

- **TransferSubscriptions is OUT of scope.** It is **not** a DataChange-facet CU — it is the
  `Subscription Transfer` CU of the **Client Redundancy Facet** (live API id 504). It is not
  required by the Embedded UA Server Profile. (OPC-10000-4 §5.14.7 remains its service section,
  but the Embedded profile does not require it.) The draft spec/plan led with it — removed.
- **SetTriggering is IN scope** (`Monitor Triggering`, §5.13.5) — the draft had wrongly listed
  it as out of scope.
- **Percent Deadband is OUT** — it is the `Data Access PercentDeadband` CU of the **Data Access
  Server Facet** (OPC-10000-8), not the DataChange facet. Absolute deadband only.
- **DataChangeFilter/DeadbandType section is §7.22.2** in current OPC-10000-4 (the codebase's
  existing `§7.17.2` citations reflect an older numbering; new code cites §7.22.2 and a note
  reconciles the two).
- **No CU exists** for a "minimum sampling interval" or "all-three-timestamps" in any DataChange
  facet — do **not** treat these as Standard-tier requirements.

## Decision 4 — Base Info Type System (the address-space delta)

- **Decision**: Implement the `Base Info Type System` CU: expose a Type System with DataTypes,
  ReferenceTypes, ObjectTypes and VariableTypes, including all OPC Foundation (namespace 0)
  types **that the server actually uses**, as defined in OPC-10000-5.
- **Rationale (verbatim CU)**: *"The Server exposes a Type System with DataTypes,
  ReferenceTypes, ObjectTypes and VariableTypes including all of the OPC Foundation provided
  (namespace 0) types that are used by the Server, as defined in Part 5."* This is the single
  mandatory address-space addition at the Embedded tier (absent from Nano/Micro). Nano/Micro
  only require `Base Info Core Structure` (Server object + ServerCapabilities + AddressSpace
  structure); Micro explicitly does *not* require a complete type system.
- **Scope note**: "types that are used by the Server" bounds the work — we expose the
  namespace-0 ReferenceType/DataType/ObjectType/VariableType nodes underpinning the nodes the
  server actually exposes (Server object hierarchy, the exposed variables' data types, the
  reference types used), not the entire Part 5 NodeSet.
- **Optional CUs NOT taken**: `Base Info Engineering Units`, `Base Info Placeholder Modelling
  Rules` (optional at Embedded); `Base Info Diagnostics`, `Base Info Server Capabilities`
  (these become required only at the **Standard UA Server Profile**, the next tier up — out of
  scope here).

## Decision 5 — Security

- **Decision**: No new security work. Basic256Sha256 (Sign / Sign&Encrypt) is already shipped
  and is the 2017-generation requirement; the `Security Default ApplicationInstance Certificate`
  CU is satisfied by the existing certificate handling. US (conformance) documents/verifies this.
- **Rationale**: Security is exactly what Embedded adds over Micro, and it is already present;
  targeting the original (non-2017) Embedded UA profile would instead require Basic128Rsa15,
  which is rejected (Decision 1).

## Decision 6 — Capacities (footprint-significant; mandated, not optional)

- **Decision**: For the `embedded` build only, raise the subscription-engine capacity defaults
  to meet the Standard facet minimums: `MU_MAX_MONITORED_ITEMS` ≥ 100, `MU_MAX_SUBSCRIPTIONS`
  ≥ 2 (already 2), `MU_MAX_PUBLISH_REQUESTS` ≥ 5, and `MU_MONITORED_QUEUE_DEPTH` ≥ 2. These are
  passed as `-D` overrides in the `embedded` Makefile/CMake wiring so the `micro`/`nano` builds
  keep their small footprint (Embedded DataChange facet only needs `Monitor Items 2`,
  `QueueSize_1`, `Subscription Minimum 1`).
- **Rationale**: `Monitor Items 100`, `Monitor MinQueueSize_02`, `Subscription Minimum 02`, and
  `Subscription Publish Min 05` are *required* CUs of the Standard facet. ≥100 monitored items is
  a material RAM increase (~12–16 KiB of fixed `struct mu_server` storage) but is well within the
  RP2040's 264 KiB and is mandatory for the claim. Capacities remain `-D`-overridable; an
  integrator who lowers them below the minimums forfeits the Standard-facet claim (documented).
- **Alternatives considered**: keeping the Micro defaults (8 items) — rejected: fails
  `Monitor Items 100`. A dynamically-sized pool — rejected: violates the no-heap memory model.

## Consolidated normative-citation map (for tasks/implementation)

| Capability | OPC-10000 citation |
|---|---|
| Call service (Method Call) | OPC-10000-4 §5.12.2.2 (CallRequest/CallResponse/CallMethodResult) |
| GetMonitoredItems / ResendData methods | OPC-10000-5 §9.1 / §9.2 (Server object methods); behavior via Call §5.12.2.2 |
| SetTriggering service | OPC-10000-4 §5.13.5 |
| CreateMonitoredItems / MonitoringParameters (queueSize, discardOldest) | OPC-10000-4 §5.13.2 |
| DataChangeFilter / DeadbandType=Absolute | OPC-10000-4 §7.22.2 |
| Overflow / NotificationMessage InfoBits | OPC-10000-4 §7.20 (NotificationMessage), §7.20.1 (overflow) |
| Subscription minimums / parallel Publish / Republish | OPC-10000-4 §5.14.2, §5.14.5, §5.14.6 |
| Base Info Type System (node set) | OPC-10000-5 (standard NodeSet); OPC-10000-3 (address-space model) |
| UA Binary encoding / status-code encoding / UA-TCP | OPC-10000-6 |
| Profile / facet / CU membership | OPC-10000-7 §6.6.69 (Embedded 2017), §6.6.17 (Standard DataChange 2017 facet), §6.6.15 (Embedded DataChange facet), §4.2 (CUs/Groups) |

## Confidence / residual gaps

- **High (directly extracted from OPC sources)**: all facet CU memberships (§6.6.15–6.6.19),
  Embedded profile composition (§6.6.68), Micro/Nano composition, every CU semantic, the fact
  that TransferSubscriptions = Client Redundancy Facet and percent-deadband = Data Access Facet.
- **Medium (triangulated)**: the Embedded **2017** profile's exact §6.6.69 row list and its use
  of the §6.6.17 (vs §6.6.16) DataChange facet — no Wayback snapshot of §6.6.69 exists; inferred
  from the consistent "2017" uplift pattern (§6.6.17/§6.6.19/§6.6.71). The user has chosen full
  2017 conformance, which is the superset, so this gap does not under-scope the work.
- **Action**: Codex (which has the OPC MCP) and the eventual CTT run will re-verify exact CU
  pass/fail; this research is sufficient to scope tasks and write tests.
