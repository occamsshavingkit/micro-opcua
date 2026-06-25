# Feature Size Ledger

This document tracks the cumulative memory footprint of the server against the hard limits defined in the architecture plan.

## Global Budgets
- **Maximum Flash**: 128 KiB
- **Maximum Static RAM**: 32 KiB
- **Dynamic Allocation (Heap)**: 0 bytes (forbidden in hot path)

## Ledger

| Date | Feature/Component | Flash Impact (bytes) | RAM Impact (bytes) | Cumulative Flash | Cumulative RAM | Notes |
|------|-------------------|----------------------|--------------------|------------------|----------------|-------|
