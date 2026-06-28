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
