#!/usr/bin/env python3
"""Feature 028 claim->test map enforcement.

For the profile a build targets, every claimed conformance unit whose row lists
that profile MUST have a backing test that is registered in that build. A claimed
unit with no profile-runnable backing test fails (nonzero exit), naming the unit.
This replaces the previous markdown-substring "traceability" with an enforced
"claimed conformance unit => a test that runs in that profile" link
(OPC-10000-7 sections 4.2 / 4.3).

Usage:
  check_claim_map.py --manifest <claim_test_map.md> --profile <p> --build-dir <dir>
"""
import argparse
import re
import subprocess
import sys

VALID_PROFILES = {"nano", "micro", "embedded", "full"}


def parse_manifest(path):
    """Return list of (claim, section, profiles:set, backing_tests:list) rows."""
    rows = []
    with open(path, encoding="utf-8") as fh:
        for line in fh:
            line = line.rstrip("\n")
            if not line.startswith("|"):
                continue
            cells = [c.strip() for c in line.strip().strip("|").split("|")]
            if len(cells) < 4:
                continue
            claim, section, profiles_cell, backing_cell = cells[0], cells[1], cells[2], cells[3]
            # Skip the header and separator rows.
            if claim.lower().startswith("claim") or set(claim) <= set("-: "):
                continue
            if profiles_cell.strip().lower() in ("all", ""):
                profiles = set(VALID_PROFILES) if profiles_cell.strip().lower() == "all" else set()
            else:
                profiles = {p.strip() for p in profiles_cell.split(",") if p.strip()}
            backing = [re.sub(r"[`\s]", "", t) for t in backing_cell.split(",") if t.strip()]
            rows.append((claim, section, profiles, backing))
    return rows


def registered_tests(build_dir):
    """Set of ctest-registered test names in the build."""
    out = subprocess.run(
        ["ctest", "-N", "--test-dir", build_dir],
        capture_output=True, text=True, check=True,
    ).stdout
    names = set()
    for line in out.splitlines():
        m = re.search(r"Test\s+#\d+:\s+(\S+)", line)
        if m:
            names.add(m.group(1))
    return names


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--manifest", required=True)
    ap.add_argument("--profile", required=True)
    ap.add_argument("--build-dir", required=True)
    args = ap.parse_args()

    if args.profile not in VALID_PROFILES:
        print(f"claim-map: unknown profile '{args.profile}'", file=sys.stderr)
        return 2

    rows = parse_manifest(args.manifest)
    if not rows:
        print("claim-map: manifest has no claim rows (parse failure?)", file=sys.stderr)
        return 2

    registered = registered_tests(args.build_dir)
    gaps = []
    applicable = 0
    for claim, section, profiles, backing in rows:
        if not profiles or args.profile not in profiles:
            continue
        applicable += 1
        if not backing:
            gaps.append(f"  [{claim}] ({section}) — no backing test listed")
            continue
        missing = [t for t in backing if t not in registered]
        if missing:
            gaps.append(
                f"  [{claim}] ({section}) — backing test(s) not registered in "
                f"profile '{args.profile}': {', '.join(missing)}"
            )

    if gaps:
        print(
            f"claim-map FAIL: {len(gaps)} claimed conformance unit(s) lack a "
            f"profile-runnable backing test in profile '{args.profile}':",
            file=sys.stderr,
        )
        for g in gaps:
            print(g, file=sys.stderr)
        print(
            "\nEach claim must have a test registered in this profile, or the "
            "claim must be corrected (OPC-10000-7 §4.2/§4.3).",
            file=sys.stderr,
        )
        return 1

    print(
        f"claim-map OK: {applicable} conformance unit(s) claimed for profile "
        f"'{args.profile}' each have a registered backing test."
    )
    return 0


if __name__ == "__main__":
    sys.exit(main())
