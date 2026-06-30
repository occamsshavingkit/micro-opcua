# Static Analysis Validation

- Tools:
  - `clang-format` 18.1.3
  - `cppcheck` 2.13.0
  - `clang-tidy` 18.1.3

Feature 005 local commands:

```sh
cmake -S . -B build/test -DMICRO_OPCUA_BUILD_TESTS=ON

files=$(
  { git diff --name-only --diff-filter=ACM -- src include tests;
    git ls-files --others --exclude-standard -- src include tests; } |
  sort -u | rg '\.(c|h)$'
)
printf '%s\n' "$files" | xargs --no-run-if-empty clang-format -i
printf '%s\n' "$files" | xargs --no-run-if-empty clang-format --dry-run --Werror

cmake --build build/test --target cppcheck

files=$(
  { git diff --name-only --diff-filter=ACM -- src include tests;
    git ls-files --others --exclude-standard -- src include tests; } |
  sort -u | rg '\.c$'
)
printf '%s\n' "$files" |
  xargs --no-run-if-empty clang-tidy -p build/test \
    --checks=-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling \
    --quiet
```

Results:

- Feature-scope `clang-format` dry-run: passed after formatting changed C/H files.
- Repo-wide `cppcheck` target: passed.
- Feature-scope `clang-tidy`: passed; remaining generated warnings are suppressed
  non-user/header diagnostics or the freestanding C project's intentional use of bounded
  `memcpy`/`memset` rather than optional C11 Annex K APIs.

Notes:

- `.clang-format` was corrected from invalid `Standard: c11` to `Standard: Cpp11` for
  clang-format 18 compatibility.
- `cmake/MicroOpcUaStaticAnalysis.cmake` was corrected so the generated analysis targets quote
  paths safely and so `cppcheck` focuses on warning/performance/portability findings instead of
  style-only noise.
- The repo-wide `format-check` target now executes correctly, but it still reports pre-existing
  formatting debt outside the Feature 005 changed-file set. Feature 005 validation used the
  changed-file check above to avoid an unrelated whole-repository reformat.

Feature 020 / T098c historical local commands:

```sh
cmake -S . -B build/test -DMICRO_OPCUA_BUILD_TESTS=ON

files=$(
  { git diff --name-only --diff-filter=ACM -- src include tests;
    git ls-files --others --exclude-standard -- src include tests; } |
  sort -u | rg '\.(c|h)$'
)
printf '%s\n' "$files" |
  xargs --no-run-if-empty clang-format --dry-run --Werror

cmake --build build/test --target cppcheck

files=$(
  { git diff --name-only --diff-filter=ACM -- src include tests;
    git ls-files --others --exclude-standard -- src include tests; } |
  sort -u | rg '\.c$'
)
printf '%s\n' "$files" |
  xargs --no-run-if-empty clang-tidy -p build/test \
    --checks=-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling \
    --quiet

cmake --build build/test --target format-check
```

Feature 020 / T098c historical results:

| Command | Exit | Result | Evidence |
| --- | ---: | --- | --- |
| `cmake -S . -B build/test -DMICRO_OPCUA_BUILD_TESTS=ON` | 0 | PASS | `/tmp/micro-opcua-t098c/01-cmake-configure.transcript` |
| Feature-scope `clang-format --dry-run --Werror` | 123 | HISTORICAL FAIL | `/tmp/micro-opcua-t098c/02-feature-clang-format-dry-run.transcript`; 734 formatting violations across 29 files before T102b formatting. |
| `cmake --build build/test --target cppcheck` | 0 | PASS | `/tmp/micro-opcua-t098c/03-cppcheck-target.transcript`; only informational `toomanyconfigs` |
| Feature-scope `clang-tidy -p build/test --checks=-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling --quiet` | 0 | ADVISORY | `/tmp/micro-opcua-t098c/04-feature-clang-tidy.transcript`; 18 explicit warnings |
| `cmake --build build/test --target format-check` | 2 | HISTORICAL FAIL | `/tmp/micro-opcua-t098c/05-repo-format-check.transcript`; 858 formatting violations across 41 files before T102b formatting. |

Historical T098c gate status: FAIL before blocker-remediation formatting. Both
the feature-scope `clang-format` dry-run and repo-wide `format-check` failed.
`cppcheck` passed, and `clang-tidy` completed with advisory analyzer warnings.

The mutating `clang-format -i` command was intentionally not run for T098c because the
validation run was non-mutating and source edits were outside the task scope.

Feature 020 / T102b blocker-remediation results:

| Command | Exit | Result | Evidence |
| --- | ---: | --- | --- |
| Feature-scope `clang-format --dry-run --Werror` | 0 | PASS | Rerun after formatting all C/H files under `src`, `include`, and `tests`; no diagnostics emitted. |
| `cmake --build build/test --target format-check` | 0 | PASS | Target printed `Check formatting with clang-format` and `Built target format-check`. |
| `cmake --build build/test --target cppcheck` | 0 | PASS | Checked 152 files; no warning/performance/portability/error diagnostics; only informational `toomanyconfigs`. |

Current T102b gate status: PASS for required formatting and `cppcheck` gates.
`clang-tidy` remains advisory using the T098c/T092g warning transcripts until a
separate warning-cleanup task is scheduled.
