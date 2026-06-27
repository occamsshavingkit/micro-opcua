# Feature Specification: Embedded Profile Completion

**Feature Branch**: `005-embedded-profile-completion`  
**Created**: 2026-06-27  
**Status**: Draft (scope confirmed by Phase-0 research, see `research.md`)  
**Input**: User description: "Complete the `embedded` build into a conformant OPC UA Embedded 2017 Device Server profile. Today `embedded` is 'Micro + SecurityPolicy Basic256Sha256' but is explicitly preliminary (see docs/conformance/status.md): it is not a complete/CTT-targeting Embedded profile. Close that gap by implementing the remaining Embedded-profile requirements relative to what already ships (Nano + Micro data-change subscriptions + Basic256Sha256), maintaining strict fidelity to the OPC-UA specification via the OPC Foundation reference (MCP) — every service/facet/encoding/StatusCode/conformance-unit decision must cite exact OPC-10000 part/section numbers. Determine the authoritative Embedded 2017 Device Server profile membership and its required conformance units from OPC-10000-7 ... and keep the freestanding C11, no-heap, no-mutable-globals, bounded-execution embedded design and size discipline intact. Out of scope unless OPC-10000-7 requires it for this profile: PubSub, alternate encodings/transports, Methods/Write/History, alarms & conditions."

## Context & Current State

micro-opcua ships three build profiles. `nano` and `micro` target the **Nano** and
**Micro Embedded Device 2017 Server Profiles** with all in-scope conformance units
implemented and CI-covered. The `embedded` build is **preliminary**: it is the Micro
surface plus SecurityPolicy Basic256Sha256. Per `docs/conformance/status.md` it is
explicitly **not** a complete OPC UA Embedded Device Server profile.

This feature closes that gap, raising `embedded` to a conformant, CTT-targeting **Embedded
2017 UA Server Profile** (`http://opcfoundation.org/UA-Profile/Server/EmbeddedUA2017`,
OPC-10000-7 §6.6.69), while preserving the freestanding C11, no-heap, no-mutable-globals,
bounded-execution design and the project's size discipline.

**Authoritative profile membership** (pinned in `research.md` from the OPC Foundation
Profile Reporting API + OPC-10000-7 v1.04 facet tables). The Embedded 2017 UA Server
Profile = the already-shipped Micro profile + the already-shipped Basic256Sha256 security
+ two additions this feature delivers:

1. The **Standard DataChange Subscription 2017 Server Facet** (§6.6.17) over the Micro
   tier's Embedded DataChange Subscription Server Facet (§6.6.15). Its required delta is:
   SetTriggering, absolute-deadband DataChangeFilter, monitored-item queue depth ≥ 2 with
   discardOldest, raised capacities (≥ 10 then ≥ 100 monitored items, ≥ 2 subscriptions,
   ≥ 5 parallel Publish), **and** — in the 2017 variant — the Call service with the
   GetMonitoredItems and ResendData methods.
2. The **Base Info Type System** conformance unit (full namespace-0 DataType / ReferenceType
   / ObjectType / VariableType exposure, OPC-10000-5).

> **Scope corrections from research** (these override the first draft):
> **TransferSubscriptions is OUT** — it belongs to the Client Redundancy Facet, not the
> DataChange facet, and the Embedded profile does not require it. **SetTriggering is IN**
> (`Monitor Triggering` CU). **Percent deadband is OUT** (Data Access Facet). The
> minimal **Method** surface (Call + GetMonitoredItems + ResendData) **is IN** by user
> decision (full Embedded 2017 conformance).

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Standard DataChange Subscription Server Facet delta (Priority: P1)

A client relying on the **Standard** DataChange Subscription Server Facet can do what the
lighter Embedded facet (already in Micro) does not: apply an absolute-deadband data-change
filter on numeric items, use a notification queue deeper than one with discard-oldest
overflow, link monitored items with SetTriggering, and rely on the larger monitored-item /
subscription / parallel-Publish capacities the Standard facet mandates.

**Why this priority**: The Standard DataChange Subscription Server Facet is the defining
capability separating the Embedded profile from the Micro profile this codebase already
meets. It is the MVP slice — built directly on the existing no-heap subscription engine.

**Independent Test**: Drive the embedded build with: a CreateMonitoredItems carrying a
DataChangeFilter with DeadbandType = Absolute on a numeric variable; a monitored item with
queueSize > 1 and discardOldest; a SetTriggering call linking a triggering item to triggered
items; and subscription/monitored-item counts up to the mandated minimums — and confirm the
server honors them with the OPC-10000-4 responses, while `micro` behaves as before.

**Acceptance Scenarios**:

1. **Given** a monitored item on a numeric variable with a DataChangeFilter whose DeadbandType
   is Absolute and a non-zero deadband (OPC-10000-4 §7.22.2), **When** the value changes by less
   than the deadband, **Then** no DataChangeNotification is queued; **When** it changes by at
   least the deadband, **Then** a notification is queued and published.
2. **Given** a monitored item with queueSize > 1 and discardOldest = true (OPC-10000-4 §5.13.2
   MonitoringParameters), **When** more changes occur than the queue holds between publishes,
   **Then** the oldest queued values are discarded, the overflow InfoBit is set (§7.20.1), and
   the most recent values are delivered.
3. **Given** a triggering monitored item and one or more triggered (sampling-only) items,
   **When** the client calls SetTriggering (OPC-10000-4 §5.13.5) to link them, **Then** a
   reported change on the triggering item causes the triggered items' samples to be reported,
   and the per-link StatusCodes are returned.
4. **Given** a client creating subscriptions/monitored items up to the Standard facet minimums
   (≥ 2 subscriptions, ≥ 10 items/subscription, ≥ 100 items total, ≥ 5 parallel Publish),
   **When** within those limits, **Then** the server accepts them; **When** a limit is exceeded,
   **Then** the OPC-10000-4 StatusCode is returned (e.g. `Bad_TooManyMonitoredItems`,
   `Bad_TooManySubscriptions`, `Bad_TooManyPublishRequests`).

---

### User Story 2 - Base Info Type System exposure (Priority: P2)

A client (including the CTT) that browses the `embedded` server finds the standard
information-model **Type System** the profile mandates: the namespace-0 DataType,
ReferenceType, ObjectType, and VariableType nodes the server uses, resolvable
HasTypeDefinition references from instances to their types, and a Server object whose
ServerProfileArray advertises the Embedded profile.

**Why this priority**: `Base Info Type System` is the single mandatory address-space addition
at the Embedded tier (Nano/Micro require only `Base Info Core Structure`). It is largely
additive static const node data, so it is lower-risk than the subscription facet but required
for a complete, CTT-passable Embedded profile.

**Independent Test**: Browse from Root/Types and confirm the mandated namespace-0
ReferenceType/DataType/ObjectType/VariableType nodes the server uses are present and reachable
with correct attributes; follow an instance's HasTypeDefinition to its type; read the Server
object's ServerProfileArray and confirm it advertises the Embedded profile URI.

**Acceptance Scenarios**:

1. **Given** a connected client, **When** it browses the Types hierarchy, **Then** the
   namespace-0 DataType / ReferenceType / ObjectType / VariableType nodes the server uses are
   present with correct NodeClass, BrowseName, and HasSubtype links (OPC-10000-5; OPC-10000-3).
2. **Given** an exposed instance node, **When** the client follows its HasTypeDefinition
   reference, **Then** it resolves to the corresponding standard type node (OPC-10000-3 §7.7).
3. **Given** a connected client, **When** it reads Server.ServerArray / ServerProfileArray,
   **Then** the values are present and the ServerProfileArray advertises the Embedded 2017 UA
   Server Profile URI.

---

### User Story 3 - Method Call surface: GetMonitoredItems & ResendData (Priority: P3)

A client can invoke the two server methods the Standard DataChange Subscription 2017 facet
requires: **GetMonitoredItems** (to recover the server/client handle pairs of a subscription's
monitored items, e.g. after reconnect) and **ResendData** (to request that a subscription
re-report all its monitored items' current values), via the **Call** service. Calls to any
other method are rejected with the cited StatusCode.

**Why this priority**: The Call service was previously out of scope; the §6.6.17 "2017" facet
makes Call + GetMonitoredItems + ResendData required, so full Embedded 2017 conformance (the
chosen target) needs them. It is P3 because it is a small, well-bounded surface that depends on
the Type System (US2) exposing the method nodes and on the subscription engine (US1).

**Independent Test**: Call GetMonitoredItems on a subscription that has monitored items and
confirm the returned serverHandles/clientHandles match; call ResendData and confirm the next
Publish re-reports current values; call an unknown method and confirm the cited StatusCode.

**Acceptance Scenarios**:

1. **Given** a subscription with monitored items, **When** the client invokes GetMonitoredItems
   for it via Call (OPC-10000-4 §5.11), **Then** the response returns matching serverHandles[]
   and clientHandles[].
2. **Given** a subscription, **When** the client invokes ResendData for it, **Then** the next
   Publish for that subscription re-reports the current value of every monitored item.
3. **Given** a Call to a method NodeId the server does not implement, **When** it is dispatched,
   **Then** the server returns the cited StatusCode (e.g. `Bad_MethodInvalid` /
   `Bad_NotImplemented`), and a Call with the wrong object/argument shape returns the cited
   per-argument StatusCode (OPC-10000-4 §5.11).

---

### User Story 4 - Conformance declaration & CTT-readiness (Priority: P4)

An integrator or auditor finds the `embedded` build wired to enable exactly the Embedded
profile's required surface, advertising consistent endpoints/profile URIs, with conformance
documentation enumerating each implemented conformance unit and its OPC-10000 citation at
status `profile-targeting` — so a CTT run can be performed and recorded without further code
archaeology.

**Why this priority**: Lowest-risk, mostly documentation and build wiring; depends on
US1–US3 to describe accurately. Converts the implemented capability into a defensible claim,
matching the project's no-claim-without-evidence policy.

**Independent Test**: Build `embedded` and confirm it advertises the Embedded profile's
security and capability surface (ServerProfileArray, capacities); review `docs/conformance/`
and confirm an Embedded-profile page enumerates each implemented CU with OPC-10000 citations
and `profile-targeting` status; confirm `nano`/`micro` are unchanged.

**Acceptance Scenarios**:

1. **Given** the `embedded` build, **When** an integrator inspects its advertised endpoints and
   ServerProfileArray, **Then** they are consistent with the Embedded profile's required facets
   and SecurityPolicy surface.
2. **Given** the conformance documentation, **When** an auditor reads the Embedded-profile page,
   **Then** every implemented Embedded conformance unit is listed with its exact OPC-10000
   citation and a status of `profile-targeting` (CTT-pending).

---

### Edge Cases

- **Capacity boundaries**: When subscriptions, monitored items, queued notifications, or parked
  Publish requests reach the Standard facet's fixed capacities, the server MUST return the
  OPC-10000-4 StatusCode (`Bad_TooManyMonitoredItems`, `Bad_TooManySubscriptions`,
  `Bad_TooManyPublishRequests`, `Bad_TooManyOperations`) rather than truncating silently or
  exceeding bounded execution.
- **Deadband on non-numeric data**: an absolute-deadband DataChangeFilter on a non-numeric
  value MUST be rejected with the cited StatusCode (`Bad_FilterNotAllowed` /
  `Bad_MonitoredItemFilterUnsupported`, §7.22.2). Percent deadband MUST be rejected
  (`Bad_MonitoredItemFilterUnsupported`) — it is not in this profile.
- **SetTriggering edge conditions**: linking unknown triggering/triggered item ids, or items in
  different subscriptions, MUST return the per-link StatusCodes defined in §5.13.5.
- **Call edge conditions**: Call to an unknown method, wrong objectId, or wrong input-argument
  count/type MUST return the cited StatusCode(s) (§5.11) and MUST NOT read out of bounds.
- **Malformed encodings**: malformed Binary encodings, invalid NodeIds, invalid ExtensionObjects,
  and invalid array/string lengths on any new request MUST be rejected with the cited StatusCode
  and MUST NOT access memory out of bounds.
- **Address-space growth vs. footprint**: the added type-system nodes MUST be exposed without
  heap use or mutable global state and within the size budget; cost MUST be measured/recorded.
- **Profile isolation**: enabling the Embedded surface MUST NOT regress the `nano`/`micro`
  builds' surface or footprint.

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: The `embedded` build MUST implement the **Standard DataChange Subscription 2017
  Server Facet** (OPC-10000-7 §6.6.17) over the Micro tier's Embedded DataChange facet, i.e.
  FR-002 through FR-008.
- **FR-002**: The server MUST support a DataChangeFilter with DeadbandType = Absolute on numeric
  monitored items (OPC-10000-4 §7.22.2), suppressing changes below the deadband and reporting
  changes at or above it; status changes always pass regardless of deadband.
- **FR-003**: The server MUST support monitored-item notification queues with queueSize ≥ 2 and
  the discardOldest policy with overflow InfoBit signaling (OPC-10000-4 §5.13.2 / §7.20.1).
- **FR-004**: The server MUST support the SetTriggering service (OPC-10000-4 §5.13.5), linking a
  triggering monitored item to triggered (sampling-only) items and returning per-link results.
- **FR-005**: The server MUST advertise and enforce capacities meeting the Standard facet
  minimums: ≥ 2 subscriptions/session, ≥ 10 monitored items/subscription and ≥ 100 monitored
  items total, ≥ 5 parallel Publish requests/session, queue depth ≥ 2 — returning the cited
  OPC-10000-4 StatusCodes when exceeded.
- **FR-006**: The server MUST support the Call service (OPC-10000-4 §5.11) for the
  GetMonitoredItems method (returning serverHandles[]/clientHandles[] of a subscription's
  monitored items) and the ResendData method (re-reporting a subscription's current values on
  the next Publish), with both methods defined per OPC-10000-5.
- **FR-007**: Call to any method the server does not implement, or with an invalid object/argument
  shape, MUST return the cited OPC-10000-4 §5.11 StatusCode(s) without out-of-bounds access.
- **FR-008**: The server MUST expose the `Base Info Type System` conformance unit: the namespace-0
  DataType, ReferenceType, ObjectType, and VariableType nodes the server uses, as static,
  browseable, readable nodes (OPC-10000-5; OPC-10000-3), with HasSubtype links among types.
- **FR-009**: Instance nodes MUST carry resolvable HasTypeDefinition references to the exposed
  standard type nodes (OPC-10000-3 §7.7).
- **FR-010**: The Server object MUST expose a ServerProfileArray advertising the Embedded 2017 UA
  Server Profile URI and a ServerCapabilities consistent with the server's actual limits
  (OPC-10000-5).
- **FR-011**: Enabling the Embedded surface MUST NOT alter the `nano`/`micro` builds' surface,
  footprint, or behavior (profile isolation via `MICRO_OPCUA_*` build options and `-D` capacity
  overrides applied only to the `embedded` build).
- **FR-012**: All new request handling (DataChangeFilter deadband, SetTriggering, Call) MUST
  reject malformed input (Binary encoding, NodeId, ExtensionObject, array/string length) with the
  cited StatusCode and MUST NOT access memory out of bounds.
- **FR-013**: `docs/conformance/` MUST gain an Embedded-profile page enumerating every implemented
  Embedded conformance unit with exact OPC-10000 citations at `profile-targeting` (CTT-pending)
  status, and `status.md` MUST be updated to reflect that `embedded` now targets the full profile.
- **FR-014**: The size ledger (`docs/size/feature-size-ledger.md`) MUST be refreshed with the
  post-feature `embedded` flash/RAM figures (including the raised capacities and type-system
  nodes), confirming `.bss` = 0 and heap = 0.

### OPC UA Normative Scope *(mandatory for protocol features)*

- **OPC-001**: Target profile is the **Embedded 2017 UA Server Profile**
  (`http://opcfoundation.org/UA-Profile/Server/EmbeddedUA2017`, OPC-10000-7 §6.6.69), building on
  the Micro Embedded Device 2017 Server Profile. Required additions: the Standard DataChange
  Subscription 2017 Server Facet (§6.6.17), the `Base Info Type System` CU, and SecurityPolicy
  Basic256Sha256 (already shipped). Authoritative membership: see `research.md`.
- **OPC-002**: Implemented services/features added, with citations:
  - SetTriggering — OPC-10000-4 §5.13.5.
  - DataChangeFilter absolute Deadband — OPC-10000-4 §7.22.2 (DeadbandType = Absolute).
  - MonitoringParameters queueSize ≥ 2 / discardOldest / overflow — OPC-10000-4 §5.13.2, §7.20.1.
  - Call service — OPC-10000-4 §5.11; GetMonitoredItems / ResendData methods — OPC-10000-5.
  - Base Info Type System node exposure — OPC-10000-5 (standard NodeSet), OPC-10000-3 (model).
  - (Already shipped, carried forward) Embedded DataChange facet service set — §5.13/§5.14.
- **OPC-003**: Unsupported services/features and capacity-exceeded conditions MUST return the
  cited OPC-10000-4 StatusCodes (`Bad_ServiceUnsupported`, `Bad_TooManyMonitoredItems`,
  `Bad_TooManySubscriptions`, `Bad_TooManyPublishRequests`, `Bad_TooManyOperations`,
  `Bad_FilterNotAllowed`, `Bad_MonitoredItemFilterUnsupported`, `Bad_MethodInvalid` /
  `Bad_NotImplemented`, `Bad_SubscriptionIdInvalid`), encoded per OPC-10000-6.
- **OPC-004**: Wire encoding/transport is unchanged: OPC UA Binary over OPC UA TCP / UA-SC
  (OPC-10000-6). No new encodings or transports.
- **OPC-005**: SecurityPolicy surface is the existing Basic256Sha256 (Sign / Sign&Encrypt) + None
  (OPC-10000-7). Conformance status after this feature is **profile-targeting**: no
  profile-compliance claim until a CTT run passes and is recorded (project policy).

### Scope Boundaries *(mandatory)*

- **In Scope**: The Standard DataChange Subscription 2017 facet delta (absolute-deadband filter,
  queue ≥ 2 / discardOldest, SetTriggering, raised capacities, Call + GetMonitoredItems +
  ResendData); the `Base Info Type System` node exposure; ServerProfileArray/ServerCapabilities;
  build wiring for profile isolation; and conformance + size documentation.
- **Out of Scope** (not required by the Embedded 2017 UA Server Profile, per `research.md`):
  - **TransferSubscriptions** (§5.14.7) — Client Redundancy Facet, not the DataChange facet.
  - **Percent / aggregate deadband** — Data Access Server Facet (OPC-10000-8).
  - The **Enhanced/Durable** DataChange facets (more items/queue/subs than Standard).
  - General **Write / HistoryRead-Update / Attribute Write** service sets, and arbitrary user
    Methods beyond GetMonitoredItems/ResendData.
  - PubSub; alternate encodings (JSON/XML) or transports (HTTPS/WebSockets); Alarms & Conditions
    / event subscriptions; `Base Info Diagnostics` / `Base Info Server Capabilities` /
    `Base Info Engineering Units` / placeholder modelling rules (Standard-UA-tier or optional CUs).
- **Compatibility Claim**: After this feature ships and a CTT run passes, `embedded` may claim
  the **Embedded 2017 UA Server Profile** over UA-TCP / UA-SC / UA-Binary with SecurityPolicy
  None and Basic256Sha256. Until the CTT run is recorded, only `profile-targeting` is claimed.
- **Application Headroom Goal**: The `embedded` core MUST stay within the project flash budget
  (≤ 128 KiB core, excl. board TCP/IP + crypto backend) and RAM budget (≤ 32 KiB static + peak
  stack, excl. platform TCP/IP buffers — note the raised ≥ 100-item capacity materially increases
  caller-provided `struct mu_server` storage; this is mandated by the facet and MUST be measured),
  preserving no-heap and no-mutable-global properties and leaving the RP2040-class target ample
  application headroom (264 KiB RAM).

### Key Entities *(include if feature involves data)*

- **MonitoredItem**: now additionally supports an absolute-deadband DataChangeFilter, a
  notification queue of depth ≥ 2 with discardOldest/overflow, and triggering links (as
  triggering or triggered item).
- **Subscription**: supports ResendData (re-report current values) and is queried by
  GetMonitoredItems; capacities raised per the Standard facet.
- **Standard type node**: a static namespace-0 address-space node (ReferenceType, DataType,
  ObjectType, or VariableType) the server uses, exposed for browse/read and as a
  HasTypeDefinition target.
- **Server method node**: the GetMonitoredItems and ResendData method nodes on the Server object,
  with their Input/OutputArguments, invoked via Call.
- **Server capability metadata**: ServerProfileArray (advertises the Embedded profile) and
  ServerCapabilities consistent with actual limits.

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: With an absolute-deadband DataChangeFilter, sub-threshold value changes produce
  zero notifications and at-or-above-threshold changes produce exactly the expected
  notifications, verified by an automated test.
- **SC-002**: A monitored item with queueSize ≥ 2 and discardOldest delivers the most recent N
  values with correct overflow signaling when more than N changes occur between publishes,
  verified by an automated test.
- **SC-003**: SetTriggering links cause a reported change on the triggering item to report the
  triggered items' samples, with correct per-link StatusCodes, verified by an automated test.
- **SC-004**: The server accepts ≥ 2 subscriptions, ≥ 100 monitored items, and ≥ 5 parallel
  Publish requests, and returns the cited StatusCodes when limits are exceeded, verified by an
  automated test.
- **SC-005**: GetMonitoredItems returns the correct handle pairs and ResendData causes a re-report
  on the next Publish; an unimplemented-method Call returns the cited StatusCode — verified by
  automated tests.
- **SC-006**: A browse reaches every mandated namespace-0 type node the server uses, every exposed
  instance resolves its HasTypeDefinition, and ServerProfileArray reads back the Embedded profile
  URI — verified by an automated test.
- **SC-007**: All specified malformed inputs and capacity-exceeded conditions on the new request
  types return the cited OPC UA StatusCodes; no input causes out-of-bounds access (sanitizer-clean).
- **SC-008**: The `embedded` core stays within the documented flash/RAM budget with recorded
  headroom; `nano`/`micro` builds are unchanged in surface and footprint; `.bss`/heap remain zero;
  figures recorded in the size ledger.
- **SC-009**: `docs/conformance/` enumerates every implemented Embedded conformance unit with exact
  OPC-10000 citations at status `profile-targeting`, leaving only the external CTT run to convert
  the claim to compliant.

## Assumptions

- **Profile membership is settled by `research.md`** (OPC Foundation Profile Reporting API +
  OPC-10000-7 v1.04 facet tables). The user has chosen full Embedded **2017** conformance,
  including the minimal Method surface the §6.6.17 facet requires.
- **The existing Micro subscription engine is the foundation.** The Standard facet extends the
  shipped no-heap, fixed-size subscription/MonitoredItem engine, not replaces it.
- **Capacities are mandated but `-D`-overridable.** The `embedded` build defaults to the facet
  minimums (≥ 100 items, ≥ 2 subs, ≥ 5 Publish, queue ≥ 2); an integrator may lower them but then
  cannot claim the Standard facet (documented). `nano`/`micro` keep their small defaults.
- **Security and transport are already in place.** Basic256Sha256 and UA-TCP/UA-Binary ship today;
  this feature adds no new security policy, encoding, or transport. The ApplicationInstance
  Certificate CU is satisfied by the existing certificate handling (verified in US4).
- **Deadband math may use soft-float.** Absolute deadband on Float/Double requires floating-point
  comparison; the target has no FPU, so libgcc soft-float is linked when deadband is exercised
  (bounded cost, measured into the size ledger; integer types use integer comparison).
- **CTT execution is external.** As with Nano/Micro, the CTT run is an external step; this feature
  delivers a CTT-targeting build and documentation, not the CTT result.
- **Methods are minimal.** Only GetMonitoredItems and ResendData are implemented; the Call
  dispatcher rejects all other methods. No general user-method framework is introduced.
