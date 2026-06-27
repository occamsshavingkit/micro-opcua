# Contract: MonitoredItem notification queue (queueSize > 1, discardOldest)

**Structure**: MonitoringParameters structure — OPC-10000-4 §7.21 (queueSize, discardOldest
fields); applied by the CreateMonitoredItems (§5.13.2) / ModifyMonitoredItems (§5.13.3)
services. Overflow/notification semantics — §7.20 (NotificationMessage / DataChangeNotification,
MonitoredItemNotification InfoBits), §7.20.1 (overflow bit). *(Exact subclause of the
MonitoringParameters structure verified against the OPC MCP at implementation time; the existing
codebase cites §7.21.)*
**Facet**: Standard DataChange Subscription Server Facet delta (confirm in research.md).

## MonitoringParameters fields (§5.13.2 (MonitoringParameters))

| Field | Type | Notes |
|---|---|---|
| clientHandle | UInt32 | echoed in every notification (existing) |
| samplingInterval | Duration | revised to server bounds (existing) |
| filter | ExtensionObject | DataChangeFilter (see deadband contract) |
| queueSize | UInt32 | requested notification queue depth |
| discardOldest | Boolean | true → drop oldest on overflow; false → drop newest |

## Behavior

- `queueSize` revised to `[1, MU_MONITORED_QUEUE_DEPTH]`; the revised value is returned in the
  MonitoredItemCreateResult (§5.13.2.2). Requested larger values are clamped, not rejected.
- Between publishes, qualifying samples are enqueued (after trigger + deadband). When the queue
  is full and a new sample arrives:
  - `discardOldest == true` → remove the oldest queued sample, enqueue the new one.
  - `discardOldest == false` → discard the incoming sample (keep the queue), per §5.13.2 (MonitoringParameters).
  - In either case set the **overflow** InfoBit on the appropriate MonitoredItemNotification so
    the client learns data was lost (§7.20.1).
- The publishing timer drains queued samples into a DataChangeNotification, oldest first, up to
  `maxNotificationsPerPublish`; remaining samples set `moreNotifications` (§5.14.5).

## Validation / failure (correctness gate)

- `queueSize == 0` → treated as 1 (§5.13.2 (MonitoringParameters)).
- Capacity: total queued samples are bounded by fixed arrays; no allocation. Over-capacity
  monitored-item creation → `Bad_TooManyMonitoredItems` (§5.13.2).

## Tests (test-first)

- queueSize=4, discardOldest=true: generate >4 changes between publishes → newest 4 delivered
  oldest-first, overflow bit set.
- discardOldest=false: oldest 4 retained, newest discarded, overflow bit set.
- Revised queueSize returned when request exceeds `MU_MONITORED_QUEUE_DEPTH`.
- maxNotificationsPerPublish split sets moreNotifications and delivers the rest next publish.
