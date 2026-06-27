# Data Model: Embedded Profile Completion

All entities are fixed-size and live in the caller-provided `struct mu_server` storage (no heap,
no mutable globals). This describes the *changes* to the existing entities (in
`src/services/subscription.h` and `src/address_space/`) for the Standard DataChange Subscription
2017 facet + Base Info Type System. Field shapes are illustrative; exact names/sizes are finalized
during implementation and measured into the size ledger. Authoritative scope: `research.md`.

## 1. MonitoredItem (extend `mu_monitored_item_t`)

OPC-10000-4 §5.13 (MonitoredItem Service Set), §5.13.2 (MonitoringParameters: queueSize,
discardOldest), §5.13.5 (SetTriggering), §7.22.2 (DataChangeFilter/DeadbandType), §7.20.1 (overflow).

**Existing**: `monitored_item_id`, `subscription_id`, `client_handle`, `node_id`, `resolved_node`,
`attribute_id`, `sampling_interval_ms`, `monitoring_mode`, `trigger`, single `last_value` +
`last_status` + `has_value`/`pending`, `next_sample_ms`.

**Added fields**:
- `deadband_type` — None | Absolute (enum). Percent is **out of scope** (Data Access Facet).
  OPC-10000-4 §7.22.2 (DeadbandType).
- `deadband_value` — numeric threshold; evaluate `|new − last_reported| ≥ value` when
  `deadband_type == Absolute` and the value is numeric.
- `queue[]` — fixed ring of up to `MU_MONITORED_QUEUE_DEPTH` queued samples (value + status +
  timestamp), replacing the single `last_value`/`pending` reporting slot. §5.13.2 `queueSize`.
- `discard_oldest` — bool; on overflow drop oldest (true) vs newest (false). §5.13.2.
- `queue_overflow` — flag to set the overflow InfoBit on the next notification (§7.20.1).
- `triggered_by` / triggering link membership — represent SetTriggering links: which triggering
  item (if any) causes this item to report while in Sampling mode, and the set of items this item
  triggers. Stored as fixed-size id references (no allocation). §5.13.5 / §5.13.1.6.

**Validation rules**:
- Absolute deadband on a non-numeric value → reject create/modify with `Bad_FilterNotAllowed` /
  `Bad_MonitoredItemFilterUnsupported` (§7.22.2 / §5.13.2.2). Percent deadband →
  `Bad_MonitoredItemFilterUnsupported`.
- `queueSize` revised to `[1, MU_MONITORED_QUEUE_DEPTH]`; larger requests clamped, revised value
  returned (§5.13.2.2). `queueSize == 0` → treated as 1.
- Negative/NaN deadband value → cited Bad StatusCode.
- SetTriggering links must be within the same subscription; unknown/foreign ids → per-link
  `Bad_MonitoredItemIdInvalid` (§5.13.5).

**State transitions**: per sample, compare against the last *reported* value via trigger + deadband;
if it passes, enqueue (applying discardOldest/overflow). The publishing timer drains the queue into
a DataChangeNotification. A triggered (Sampling-mode) item's queued samples are reported when its
triggering item reports (§5.13.1.6). A ResendData request (below) forces a re-report of current
values on the next publish.

## 2. Subscription (extend behavior)

OPC-10000-4 §5.14; capacities §5.14.2; parallel Publish §5.14.5; ResendData via §5.11.

**Existing**: `subscription_id`, `session_id`, interval/keep-alive/lifetime counts, priority,
publishing flags, sequence/keep-alive/lifetime counters, `next_publish_ms`, `more_notifications`,
`retransmit` slot.

**Added/changed**:
- `resend_data_pending` — one-shot flag set by the ResendData method; on the next publish, all the
  subscription's monitored items re-report their current value, then the flag clears.
- **Capacity**: `MU_MAX_SUBSCRIPTIONS` ≥ 2 (Standard `Subscription Minimum 02`); the `embedded`
  build raises capacities via `-D` (research Decision 6); `micro`/`nano` keep small defaults.

**Validation rules**:
- GetMonitoredItems / ResendData on an unknown subscription → `Bad_SubscriptionIdInvalid` (§5.11).
- Subscription not owned by the calling session → cited StatusCode.

## 3. Triggering links (SetTriggering, §5.13.5)

Represented within the MonitoredItem entity (no separate heap structure): for a triggering item,
a fixed-size list of triggered monitored-item ids. `linksToAdd`/`linksToRemove` mutate this list;
each yields a per-link StatusCode. Bounded by a fixed `MU_MAX_TRIGGER_LINKS` (or reuse the item
array with a back-reference) — exact representation chosen in implementation, no allocation.

## 4. Call / Method entities (new wire + dispatch)

OPC-10000-4 §5.11 (Call); methods defined OPC-10000-5.

- **CallMethodRequest**: `objectId` (NodeId), `methodId` (NodeId), `inputArguments` (Variant[]).
- **CallMethodResult**: `statusCode`, `inputArgumentResults` (StatusCode[]),
  `inputArgumentDiagnosticInfos`, `outputArguments` (Variant[]).
- **GetMonitoredItems**: input `subscriptionId` (UInt32) → output `serverHandles` (UInt32[]),
  `clientHandles` (UInt32[]).
- **ResendData**: input `subscriptionId` (UInt32) → no output; sets `resend_data_pending`.

**Validation rules**: only the Server object + the two method NodeIds are accepted; other
objectId/methodId → `Bad_NodeIdInvalid` / `Bad_MethodInvalid` (or `Bad_NotImplemented`). Wrong
argument count/type → per-argument StatusCode (§5.11). Array length > fixed cap →
`Bad_TooManyOperations`. Malformed request → cited decode StatusCode; no OOB read.

## 5. Standard information-model nodes (extend `base_nodes.c`)

OPC-10000-5 (standard NodeSet — `Base Info Type System` CU); OPC-10000-3 (address-space model);
standard NodeIds per OPC-10000-6 Annex A.

**Added (bounded to "namespace-0 types the server uses", research Decision 4)**:
- Standard **ReferenceType** nodes the server uses (References, HierarchicalReferences, HasChild,
  HasComponent, HasProperty, Organizes, HasTypeDefinition, HasSubtype, …) with IsAbstract /
  Symmetric / InverseName.
- Standard **DataType** nodes underpinning exposed values (BaseDataType subtree used), HasSubtype.
- Standard **ObjectType / VariableType** nodes used (BaseObjectType, BaseVariableType,
  BaseDataVariableType, PropertyType, ServerType, …), HasSubtype.
- **HasTypeDefinition** references from exposed instances to their type nodes (§7.7).
- **GetMonitoredItems / ResendData method nodes** on the Server object (HasComponent) with
  Input/OutputArguments Properties (for US3 Call resolution).
- **ServerProfileArray** advertising the Embedded 2017 UA Server Profile URI; ServerCapabilities
  consistent with actual limits.

**Validation rules**: nodes are `const` static (flash, not RAM); resolved via the existing
`mu_resolve_node` fallback chain; adding them MUST NOT change nano/micro browse/read output (gated,
profile-scoped). Reading an unsupported attribute → `Bad_AttributeIdInvalid`; unknown node →
`Bad_NodeIdUnknown`.

## 6. Capacity / configuration macros

- `MU_MONITORED_QUEUE_DEPTH` — new, default ≥ 2 (embedded), `-D`-overridable.
- `MU_MAX_TRIGGER_LINKS` — new, bounds the SetTriggering link list.
- `embedded` build raises `MU_MAX_MONITORED_ITEMS` ≥ 100, `MU_MAX_SUBSCRIPTIONS` ≥ 2,
  `MU_MAX_PUBLISH_REQUESTS` ≥ 5 (research Decision 6); `micro`/`nano` keep small defaults.
- `MU_SERVER_STORAGE_BYTES` recomputed for `embedded` (expected ~25–30 KiB) and documented;
  checked by `mu_server_init`. `.bss`/heap stay 0.

## Entity relationships

```
Session 1..* ── owns ──> Subscription 1..* ── owns ──> MonitoredItem
                              │  ^                          │  │
              ResendData ─────┘  └── GetMonitoredItems      │  └─ SetTriggering links (§5.13.5)
              (Call §5.11)           (Call §5.11)           │     (triggering ↔ triggered, same sub)
                                                  samples ──> AddressSpace node
                                                  (deadband/queue/overflow) │
Server object ── HasComponent ──> GetMonitoredItems / ResendData methods    └─> HasTypeDefinition ─> Type node
Subscription  ── retains ──> Retransmit slot (Republish §5.14.6)
```
