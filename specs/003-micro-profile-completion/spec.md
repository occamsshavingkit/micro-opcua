# Feature Spec: Micro Embedded Device 2017 Server Profile Completion

**Feature ID**: 003-micro-profile-completion
**Status**: profile-targeting (Micro completion)
**Target Profile**: Micro Embedded Device 2017 Server Profile
(`http://opcfoundation.org/UA-Profile/Server/MicroEmbeddedDevice2017`, OPC Foundation
profile id 1659). The profile "builds upon the Nano Embedded Device Server Profile.
The most important additions are: support for subscriptions via the **Embedded Data
Change Subscription Server Facet** and support for **at least two sessions**." A
complete Type System is not required.

## Goal

Take the server from a complete **Nano** surface (connect → discover → session →
Browse → Read, interop-validated, single session) to a complete **Micro** surface by
adding (1) a no-heap **data-change subscription engine** — the Subscription Service
Set (OPC 10000-4 §5.14) and the MonitoredItem Service Set (§5.13) constrained to the
Embedded Data Change Subscription Server Facet — and (2) support for **≥2 concurrent
sessions**. Security policies, the Standard (fuller) subscription facet, and full
type-system exposure remain above Micro (the Embedded tier) and are out of scope here.

## Division of work (per request, unchanged from Nano)

- **Claude owns**: this spec, the plan, the **architecture/design**, every **test**
  (unit + integration), and all **validation** (build under `-Werror`, sanitizers,
  the dotnet-interop CI job, conformance traceability). Tests are written first (RED)
  and verified to fail before any production code, per the constitution / TDD skill.
- **Codex owns**: **implementation only** — writing the production C that makes the
  Claude-authored failing tests pass. Codex must not add or modify tests, CMake test
  wiring, specs, or docs, and must not commit/push. The Codex MCP is called with only
  `{prompt, cwd}` (no sandbox / approval-policy line).

## Architecture (Claude-owned design)

### No-heap storage model

All subscription state lives in the caller-owned `struct mu_server` (no allocation),
behind a new compile flag `MICRO_OPCUA_SUBSCRIPTIONS`. Fixed-size arrays, sized by
compile-time constants the integrator may override with `-D`:

| Constant | Default | Meaning |
|---|---|---|
| `MU_MAX_SESSIONS` | 2 | concurrent sessions (profile requires ≥2) |
| `MU_MAX_CONNECTIONS` | 2 | concurrent TCP connections / secure channels |
| `MU_MAX_SUBSCRIPTIONS` | 2 | total subscriptions across all sessions |
| `MU_MAX_MONITORED_ITEMS` | 8 | total data MonitoredItems across all subscriptions |
| `MU_MAX_PUBLISH_REQUESTS` | 4 | queued (parked) Publish requests |
| `MU_MAX_MONITORED_STRING` | 32 | bytes for a string-identifier monitored NodeId |

Adding the engine grows `sizeof(struct mu_server)`; `MU_SERVER_STORAGE_BYTES` is
raised to **2048** when `MICRO_OPCUA_SUBSCRIPTIONS` is defined (was 1024), and the
`mu_server_init` alignment/size guard continues to apply.

### Subscription object (OPC 10000-4 §5.14.1.3 state variables)

Per subscription: `subscription_id` (unique IntegerId), owning `session_id`,
`publishing_interval_ms` (revised, integer ms — no FPU; the wire Duration double is
decoded to/encoded from integer ms with a bounded helper, mirroring the
SessionTimeout bit-clamp approach), `max_keep_alive_count`, `lifetime_count`,
`max_notifications_per_publish`, `priority`, `publishing_enabled`, and the runtime
counters `sequence_number`, `keep_alive_counter`, `lifetime_counter`,
`next_publish_tick_ms`, `more_notifications`, plus a single-slot **retransmission**
buffer (last NotificationMessage: sequence number + encoded body) for Republish.

### MonitoredItem object (OPC 10000-4 §5.13, §7.21 MonitoringParameters)

Per item: owning `subscription_id`, `monitored_item_id` (unique IntegerId),
`client_handle` (echoed in notifications), monitored `node_id` (numeric, or a bounded
string id), `attribute_id` (Value=13 the common case), `sampling_interval_ms`,
`monitoring_mode` (DISABLED / SAMPLING / REPORTING, §5.13.1.2), a **DataChangeFilter**
(trigger = Status / StatusValue / StatusValueTimestamp, §7.17.2 — absolute deadband
not required by the Embedded facet and is accepted-but-ignored), and change-detection
state: `last_value` (an inline `mu_variant_t` — scalar built-in types only, which
covers the monitorable base nodes such as `ServerStatus.CurrentTime`,
`ServerStatus.State`, `ServiceLevel`), `last_status`, and `has_pending_notification`.

### Sampling and publishing are driven by `mu_server_poll`

The engine is cooperative and non-blocking like the rest of the server. Each
`mu_server_poll` iteration, after inbound processing, calls a `mu_subscriptions_tick`
that takes the current monotonic tick (`time_adapter.get_tick_ms`):

1. **Sample**: for each REPORTING/SAMPLING monitored item whose sampling timer is due,
   resolve and read the node value through the address space (`mu_resolve_node` +
   value source), compare to `last_value` per the DataChangeFilter; on a change update
   `last_value` and set `has_pending_notification`.
2. **Publish-timer**: for each subscription whose publishing timer is due
   (`next_publish_tick_ms <= now`, OPC 10000-4 §5.14.1.2 state table): if there are
   pending notifications **and** a Publish request is parked, dequeue it, build a
   **DataChangeNotification** (§7.20/§7.21) NotificationMessage with the changed items,
   send a PublishResponse, reset the keep-alive counter, and stash the message for
   Republish; else if `keep_alive_counter` reached `max_keep_alive_count` and a request
   is parked, send a **keep-alive** PublishResponse (empty NotificationMessage,
   §5.14.1.1); otherwise advance the counters.

### Asynchronous Publish (the one non-synchronous service)

Every other service is request-in → response-out within one poll. **Publish** is not:
a PublishRequest that has no notifications ready is **parked** in a fixed-size queue
(its `request_id`, `request_handle`, and `SubscriptionAcknowledgements`), acknowledged
notifications are purged from the retransmission buffers, and the response is emitted
later by `mu_subscriptions_tick` when the publishing timer fires. This requires the
chunk-framing/send path to be reachable from the tick (not only from a request
handler); the design routes a parked request's response back over the connection /
secure channel that the owning session is bound to. If the queue is full,
`Bad_TooManyPublishRequests` is returned immediately (§5.14.5.3).

### Multi-session and multi-connection (≥2 sessions)

`struct mu_server` carries `mu_session_t sessions[MU_MAX_SESSIONS]` and the connection
state becomes `mu_connection_t conns[MU_MAX_CONNECTIONS]` (each its own
`tcp_connection` + `secure_channel` + rx reassembly + idle clock). The accept/poll loop
fills a free connection slot and iterates all slots per poll. CreateSession picks a
free session slot (`Bad_TooManySessions` when full, §5.6.2); session lookup is by
`auth_token` across the array; a session is bound to the connection it was activated on
so async Publish responses route correctly. Closing a connection frees its session(s)
and their subscriptions/monitored items.

## In scope (the Embedded Data Change Subscription Server Facet + ≥2 sessions)

### US1 — Subscription lifecycle core (P1)
`MICRO_OPCUA_SUBSCRIPTIONS` flag + the subscription data model + the publish-timer
bookkeeping skeleton driven by `mu_server_poll`.
- **CreateSubscription** (OPC 10000-4 §5.14.2): assign a unique `subscriptionId`,
  revise PublishingInterval / LifetimeCount / MaxKeepAliveCount to server limits, start
  the publishing timer; `Bad_TooManySubscriptions` when full (§5.14.2.3).
- **DeleteSubscriptions** (OPC 10000-4 §5.14.8): delete each subscription and its
  MonitoredItems; per-op `Bad_SubscriptionIdInvalid` for unknown ids.

### US2 — MonitoredItems + data-change sampling (P1)
- **CreateMonitoredItems** (OPC 10000-4 §5.13.2) for `MonitoringMode` data-change only,
  with MonitoringParameters (§7.21) and an optional DataChangeFilter (§7.17.2): bind
  each item to a node/attribute, revise the sampling interval, take the initial sample
  (first value is reported), assign a `monitoredItemId`. Operation results
  `Bad_NodeIdUnknown` / `Bad_AttributeIdInvalid` / `Bad_MonitoringModeInvalid` /
  `Bad_TooManyMonitoredItems` (§5.13.2.4).
- **DeleteMonitoredItems** (OPC 10000-4 §5.13.6): remove items; `Bad_MonitoredItemIdInvalid`.
- The poll-driven **sampling + change detection** described above.

### US3 — Publish / keep-alive / Republish (P1)
- **Publish** (OPC 10000-4 §5.14.5): park requests, process SubscriptionAcknowledgements,
  emit DataChangeNotification (§7.20) or keep-alive NotificationMessages asynchronously
  from the tick; `Bad_TooManyPublishRequests` / `Bad_NoSubscription` as specified.
- **Republish** (OPC 10000-4 §5.14.6): resend the retained NotificationMessage for a
  given sequence number; `Bad_MessageNotAvailable` when not retained.

### US4 — Subscription / MonitoredItem management (P2)
- **ModifySubscription** (§5.14.3), **SetPublishingMode** (§5.14.4),
  **ModifyMonitoredItems** (§5.13.3), **SetMonitoringMode** (§5.13.4).

### US5 — At least two sessions (P1, profile-mandatory)
Multi-session + multi-connection per the architecture above; `Bad_TooManySessions`
when full; per-connection isolation of channel/session/subscription state.

### US6 — Profile wiring, conformance docs, validation (P1)
- `MICRO_OPCUA_SUBSCRIPTIONS` CMake option + `src/CMakeLists.txt` gating; `make micro`
  builds with subscriptions ON (no longer == nano); storage bump.
- `docs/conformance/{status,services}.md` + a new `profile-micro.md`; traceability
  (`docs/traceability/files-to-sections.md`) for the new sources.

## Out of scope (Embedded tier and above)
Security policies on subscriptions; the Standard/Enhanced DataChange facets (larger
limits, event subscriptions, aggregate/percent deadband filters, TransferSubscriptions,
SetTriggering); full type-system exposure; Method/Call, Write, Historical Access.

## Success criteria
- A client can CreateSession (×2 concurrently) → CreateSubscription →
  CreateMonitoredItems on a base node → receive an initial-value DataChangeNotification
  and a subsequent change notification via Publish, plus keep-alives → Delete cleanly.
- All under `-Werror`, full ctest green, ASan/UBSan clean, dotnet-interop unaffected.
- `make micro` produces a subscriptions-enabled binary distinct from `make nano`.
- No heap; `sizeof(struct mu_server)` within the raised `MU_SERVER_STORAGE_BYTES`.
- Profile-compliance is still only *claimed* after an external CTT run (project policy).
