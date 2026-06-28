# Contract: SetTriggering

**Service**: SetTriggering — OPC-10000-4 §5.13.5. Encoding: OPC-10000-6 UA Binary.
**Facet**: `Monitor Triggering` CU of the Standard DataChange Subscription 2017 Server Facet
(OPC-10000-7 §6.6.17; see research.md).
**Build gate**: `MICRO_OPCUA_SUBSCRIPTIONS` (embedded profile scope).

## Request (SetTriggeringRequest, §5.13.5.2)

| Field | Type | Notes |
|---|---|---|
| requestHeader | RequestHeader | standard |
| subscriptionId | UInt32 | subscription owning the items |
| triggeringItemId | UInt32 | the monitored item whose reporting triggers others |
| linksToAdd | UInt32[] | monitored-item ids to add as triggered |
| linksToRemove | UInt32[] | monitored-item ids to remove |

## Response (SetTriggeringResponse, §5.13.5.2)

| Field | Type | Notes |
|---|---|---|
| responseHeader | ResponseHeader | standard |
| addResults | StatusCode[] | one per linksToAdd, same order |
| addDiagnosticInfos | DiagnosticInfo[] | empty unless requested |
| removeResults | StatusCode[] | one per linksToRemove, same order |
| removeDiagnosticInfos | DiagnosticInfo[] | empty unless requested |

## Behavior (§5.13.1.6 triggering model, §5.13.5)

- A **triggering** item links to **triggered** items in the same subscription. When the
  triggering item's monitoring mode reports (or, per the triggering rules, has a value to
  report), each triggered item in MonitoringMode = Sampling reports its queued samples even
  though Sampling mode alone does not report.
- `linksToAdd`/`linksToRemove` are applied per id; each yields a per-link StatusCode.
- Triggered items keep sampling per their own samplingInterval; the link only governs *reporting*.

## Validation / failure (correctness gate)

- Unknown `subscriptionId` → `Bad_SubscriptionIdInvalid` (service-level).
- Unknown `triggeringItemId` → `Bad_MonitoredItemIdInvalid` (service-level).
- Unknown link id in add/remove → per-link `Bad_MonitoredItemIdInvalid`.
- Link to an item in a different subscription → per-link `Bad_MonitoredItemIdInvalid`.
- Empty linksToAdd and linksToRemove → `Bad_NothingToDo`.
- Array length > fixed limit → `Bad_TooManyOperations`. Malformed array/length → cited decode
  StatusCode; no out-of-bounds read.

## Tests (test-first)

- Add a triggering link; a reported change on the triggering item reports a Sampling-mode
  triggered item's samples.
- Remove the link; the triggered item stops reporting on the trigger.
- Unknown subscription/triggering/link ids → cited StatusCodes (service- and per-link).
- Over-length/malformed arrays → cited StatusCodes; ASan-clean; fuzz the decoder.
