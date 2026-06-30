# Quickstart: Audit Hardening

This quickstart describes how to validate the feature during planning, task generation, and implementation.

## 1. Confirm Active Feature

```bash
cat .specify/feature.json
git status -sb
```

Expected:

- `feature_directory` is `specs/020-audit-hardening`.
- Branch is `020-audit-hardening`.

## 2. Generate Tasks With Required Shape

When running `/speckit-tasks`, require every task to be:

- Atomic.
- Ordered by dependency.
- Test-first for protocol behavior.
- Grounded in an exact OPC UA section from `plan.md`.
- Explicit about expected StatusCode or TCP Error for negative paths.

The first task groups should cover shared malformed-array and ExtensionObject boundary tests before any service-specific implementation.

## 3. Baseline Verification Commands

```bash
cmake --build build
ctest --test-dir build --output-on-failure
cmake --build build-san
ctest --test-dir build-san --output-on-failure
./scripts/size-report.sh build/src/libmicro_opcua.a
```

## 4. Required Negative-Path Validation Themes

- Array length below `-1` rejects with `Bad_DecodingError`.
- Truncated `CreateSession` does not allocate a session.
- Declared ExtensionObject body length cannot read past the parent request.
- Missing DataValue `Value` does not reach write or history callbacks.
- Invalid `TimestampsToReturn` and BrowseDirection return exact StatusCodes.
- Unsupported MonitoringFilter and AggregateFilter options return exact per-item StatusCodes.
- HEL/ACK values obey peer-requested and OPC UA minimum buffer rules.
- Abort/non-final chunks do not dispatch service bodies.
- Session tokens are not fixed and cross-channel session use is rejected.
- Username/password over `SecurityPolicy#None` is disabled by default.

## 5. Fuzz and CI Gate Checks

After implementation work begins, ensure fuzz harnesses are meaningful and not placeholders:

```bash
rg -n "To be implemented|TODO|\\(void\\)data;|\\(void\\)size;" tests/fuzz
```

When Clang/libFuzzer is available, build fuzz targets and run short smoke campaigns. Any remaining advisory-only CI gates must be documented with a risk rationale or made blocking for release validation.

## 6. Documentation and Evidence Checks

Before closing the feature:

```bash
rg -n "profile-compliant|CTT-verified|Traceability mapped|Placeholder replaced|TBD" docs specs/020-audit-hardening
rg -n "Bad_[A-Za-z0-9_]+|OPC-10000-" specs/020-audit-hardening docs/traceability docs/conformance
```

Expected:

- No unsupported profile-compliant or CTT-verified claims.
- No placeholder traceability rows for affected files.
- Every protocol claim cites exact OPC UA sections.

## 7. Closure Matrix

The implementation phase should end with a closure matrix for every audit finding. Each row should include:

- Finding source and severity.
- Requirement id.
- OPC UA section.
- Test or verification evidence.
- Size/RAM/stack impact.
- Closure status.
