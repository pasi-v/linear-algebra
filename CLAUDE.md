# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Who writes what

The owner writes **all linear-algebra library code by hand** — anything under
`src/`, `tests/`, and `include/la/` (plus `include/math_utils/` and
`include/utils/`). Don't write or edit code in those paths unless explicitly
asked. Suggestions, reviews, refactor ideas, failing-test reproductions, and
explanations are fine and welcome; edits in those paths are not.

The `la_calc` REPL (everything under `app/`, including `app/app_checks/`) is
fair game — Claude may write and modify it as usual.

## Build & test

The project is built with `make` (clang++, C++11, `-Wall -Wextra -Wpedantic`,
`-O0 -g`). `make all` (the default) builds three binaries into `bin/`:

- `bin/tests` — doctest runner for the library (`src/` + `tests/`)
- `bin/la_calc` — the REPL app (`app/`)
- `bin/app_tests` — doctest runner for the REPL (`app/app_checks/`)

Common targets:

```bash
make            # build everything
make test       # build + run bin/tests
make app_tests  # build + run bin/app_tests
make format     # clang-format library + tests + public headers
make clean      # remove bin/ and all .o / .d files
```

There is no built-in "run a single test" target. With doctest, filter at the
binary level instead:

```bash
./bin/tests --test-case="m x n Zero Matrix()"
./bin/tests --test-case="*Matrix*"          # glob filter
./bin/tests --list-test-cases               # discover names
```

`Makefile` writes `.o` and `.d` files **next to the sources** (not in a build
dir) and includes the `.d` files for header-change tracking. `make clean`
removes them; `.gitignore` keeps them out of git.

`compile_commands.json` (for clangd) is regenerated with
`bear -- make clean all`. The hand-rolled `generate_compile_commands.sh` is
**not** the canonical path — it only scans the cwd for `*.cpp` and produces
an incomplete DB; prefer `bear`.

## Architecture

### Three-target layout

The Makefile compiles every `src/*.cpp` into a pool of library objects
(`LIB_OBJS`) and links it three ways:

- `bin/tests` ← `LIB_OBJS` + `tests/*.o`
- `bin/la_calc` ← `LIB_OBJS` + `app/*.o`
- `bin/app_tests` ← `LIB_OBJS` + `app/*.o` **minus `app/la_calc.o`** +
  `app/app_checks/*.o`

That `filter-out` on `app/la_calc.o` is load-bearing: it lets the REPL test
binary link `app/evaluator.o` and `app/parser.o` without pulling in the
real `main()`. doctest's own `main` comes from
`tests/test_main.cpp` / `app/app_checks/main.cpp`
(`DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN`).

Include roots (`-Iinclude -Ithird_party -Iapp`):

- `include/la/` — public library headers, all under `namespace la`
- `include/math_utils/`, `include/utils/` — small leaf utilities
- `third_party/doctest/` — vendored test framework
- `app/` — only on the include path so REPL TUs can `#include "evaluator.hpp"`
  etc. without `app/` prefixes; library code must not depend on `app/`.

### Library shape (`la::`)

The core types are `la::Vector` (`include/la/vector.hpp`) and `la::Matrix`
(`include/la/matrix.hpp`) — thin row-major value types. Algebra lives in
**separate free-function headers**, not as member functions:

- `vector_algorithms.hpp` — `dot`, `norm`, `angle`, `proj_onto`,
  `are_linearly_independent`, ...
- `row_reduction.hpp`, `determinant.hpp`, `matrix_transforms.hpp`,
  `matrix_linear_systems.hpp`, `pivot_info.hpp`, `pivot_policy.hpp` — matrix
  algorithms
- `matrix_algorithms.hpp` — **umbrella header that re-includes** the matrix
  algorithm headers above. Use it from app/test code; new algorithms go into
  the appropriate split header and (if broadly useful) get added to the
  umbrella.
- `linear_system.hpp` + `eliminated_system.hpp` — `solve(A, b)` returning a
  `LinearSystemSolution { kind, particular, directions }`.

The split-out structure is the result of an in-progress refactor away from
member-function algebra; see `TODO.md` "Separating Matrix core structures
and algebra functions". When adding algorithms, prefer free functions in the
matching `*_algorithms`-style header over adding members to `Matrix` /
`Vector`.

### REPL app (`app/`)

`la_calc` is a line-oriented REPL. The pipeline per line is:

1. `la_calc.cpp` — read a line, dispatch to `execute_line`.
2. `evaluator.cpp` — dispatch on the first token (`vec`, `mat`, `dot`, `det`,
   `rref`, `in_span`, `lin_indep`, `print`, `help`, `quit`/`exit`), parse the
   rest with `Parser`, call into `la::`, write to `out`/`err`.
3. `parser.cpp` — hand-written recursive-descent tokenizer for identifiers,
   numbers, and `[...]` / `[[...]]` literals.
4. `value.hpp` — `Value { Kind, la::Vector, la::Matrix }` tagged-union-ish
   struct held in the symbol table (`std::unordered_map<std::string, Value>`).

Both payloads (`vec`, `mat`) are always constructed even when only one is
used — see how `evaluator.cpp` builds `Value { Kind::Vector, v, Matrix(0, 0) }`.
Match that pattern when adding new value kinds.

REPL tests in `app/app_checks/` drive `execute_line` with streams and assert
on captured `out`/`err`; mirror that style for new commands.

### Active backlog

`TODO.md` is the source of truth for in-flight library/REPL work and known
refactors (notably the `Matrix::ref` cleanup and the member-vs-free-function
split). Check it before starting non-trivial work — items there have context
that isn't in the code.

Before changing a backlog item, verify its `TODO.md` entry against the current
code — the entry can lag reality. Once an item is done, update `TODO.md` in the
same change.

## Conventions

- C++11 only — no C++14/17 features. `requires-python`-style version pinning
  lives in `Makefile` (`-std=c++11`).
- Formatting: clang-format, LLVM base, 4-space indent, 79-col limit
  (`.clang-format`). Run `make format` before committing library/test code.
- Floating-point comparisons in the library use `math_utils::nearly_equal` /
  the helpers in `include/la/approx.hpp`; tests use doctest's `doctest::Approx`
  or the helpers in `tests/test_utils.hpp`. Don't compare doubles with `==`.
- Before suggesting a call into the library or a test helper (e.g.
  `nearly_equal`, the `CHECK_*` macros in `tests/test_utils.hpp`), confirm the
  symbol's actual signature and available overloads first — don't assume one
  exists. A scalar-vs-`Vector`/`Matrix` overload mismatch here has produced
  suggestions that didn't compile.
- Validation helpers (`utils::check_nonnegative` in `include/utils/utils.hpp`)
  throw `std::invalid_argument` / `std::out_of_range` — propagate those
  exception types from new boundary checks rather than inventing new ones.
- Exception types for precondition checks follow a fixed split:
  - `std::invalid_argument` — operands don't conform dimensionally or an
    argument is malformed (vector/matrix size mismatch, non-square where a
    square is required, augmenting matrices with differing row counts). This
    is the default for shape/conformance checks; a caller can wrap a whole
    matrix expression in one `catch (const std::invalid_argument&)`.
  - `std::domain_error` — the input is well-formed but the operation is
    mathematically undefined there (e.g. `angle` of a zero-length vector).
  - `std::out_of_range` — an index or position is outside valid bounds
    (element access, `row()`/`column()`, `set_row`/`set_col` indices).
  Don't reach for `domain_error` on a size mismatch just because the operation
  is "mathematical" — that's the inconsistency this rule exists to prevent.
- Element-access and iterator accessors come in const/non-const pairs
  (`at()`, `operator[]`, `begin()`/`end()`). Choose between checked and
  unchecked by the *provenance* of the index, not by habit: `operator[]` when
  the index is provably in range by local reasoning (a counted loop over the
  object's own `size()`/`rows()`/`cols()`, or after a conformance/bounds check
  has already thrown); `at()` at trust boundaries — caller-supplied or parsed
  indices, and computed/derived indices (e.g. pivot/free columns from
  `PivotInfo`) where a typo would be silent UB. When unsure, prefer `at()`: a
  wrong guess toward it costs a branch, a wrong guess toward `[]` is undefined
  behavior.
- Keep const/non-const accessor pairs that complete the standard
  container-like interface as a *pair*, even when one half currently has no
  caller. The safe, idiomatic surface (`at()`, `operator[]`, `begin()`/`end()`)
  is expected to be whole — a lopsided container that looks mutable-iterable
  but silently isn't is worse than an unused overload. The exception is a raw,
  unchecked escape hatch with no caller and no near-term need — e.g. the
  non-const `double *data()` — which is fair to drop under YAGNI precisely
  because it is *not* part of the safe idiomatic surface. Decide by a stated
  principle (strict-YAGNI vs complete-the-interface), not case by case.
- Don't add a test whose only purpose is to color in coverage of an
  otherwise-uncalled overload (e.g. a const-only or non-const-only accessor).
  Cover the const overload through a realistic `const Vector&`/`const Matrix&`
  caller, and let coverage of a mutable accessor close *naturally* when the
  first real mutating caller is written. If that caller never arrives, the
  persistent gap is the signal the overload was unnecessary.
