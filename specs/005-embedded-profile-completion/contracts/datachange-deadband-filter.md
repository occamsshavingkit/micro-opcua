# Contract: DataChangeFilter with absolute Deadband

**Structure**: DataChangeFilter / DeadbandType — OPC-10000-4 §7.22.2 (the codebase's existing
`§7.22.2` citations reflect an older numbering; new code cites §7.22.2). Carried in
MonitoringParameters.filter on CreateMonitoredItems (§5.13.2) / ModifyMonitoredItems (§5.13.3).
Encoding: OPC-10000-6 UA Binary (ExtensionObject).
**Facet**: `Monitored Items Deadband Filter` CU of the Standard DataChange Subscription 2017
Server Facet (OPC-10000-7 §6.6.17; see research.md).

## DataChangeFilter fields (§7.22.2)

| Field | Type | Notes |
|---|---|---|
| trigger | DataChangeTrigger | Status / StatusValue / StatusValueTimestamp (already supported) |
| deadbandType | UInt32 (enum) | None(0) / Absolute(1) / Percent(2) |
| deadbandValue | Double | threshold; meaning depends on deadbandType |

## Behavior (Absolute deadband)

- `deadbandType == None` → existing behavior (report on every trigger-qualifying change).
- `deadbandType == Absolute` on a **numeric** value → report a change only when
  `|newValue − lastReportedValue| ≥ deadbandValue` (§7.22.2). Integer types use integer
  comparison; Float/Double use soft-float comparison (no FPU on target — see plan Complexity
  Tracking). Status changes always pass regardless of deadband (trigger semantics, §7.22.2).
- `deadbandType == Percent` → **not supported** in this profile (Enhanced facet); return the
  cited StatusCode (below) unless research shows the Embedded profile mandates it.

## Validation / failure (correctness gate)

- Absolute deadband on a non-numeric value → reject create/modify of that item with
  `Bad_FilterNotAllowed` or `Bad_MonitoredItemFilterUnsupported` (exact code research-pinned,
  §7.22.2 / §5.13.2.2).
- Unsupported `deadbandType` (e.g. Percent when unsupported) → `Bad_MonitoredItemFilterUnsupported`.
- `deadbandValue` negative / NaN → cited Bad StatusCode.
- Malformed ExtensionObject / filter body → cited decode StatusCode (OPC-10000-6); no OOB read.

## Tests (test-first)

- Absolute deadband on a numeric variable: sub-threshold change → no notification;
  at/above-threshold change → exactly one notification.
- Absolute deadband on a String/Boolean variable → cited Bad StatusCode at create.
- Status-only change with a value within deadband still reports (trigger precedence).
- Float/Double deadband path exercised (soft-float) and integer path exercised separately.
- Malformed filter ExtensionObject → cited StatusCode; ASan-clean; fuzz.
