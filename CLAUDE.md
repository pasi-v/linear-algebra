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

## Conventions

- C++11 only — no C++14/17 features. `requires-python`-style version pinning
  lives in `Makefile` (`-std=c++11`).
- Formatting: clang-format, LLVM base, 4-space indent, 79-col limit
  (`.clang-format`). Run `make format` before committing library/test code.
- Floating-point comparisons in the library use `math_utils::nearly_equal` /
  the helpers in `include/la/approx.hpp`; tests use doctest's `doctest::Approx`
  or the helpers in `tests/test_utils.hpp`. Don't compare doubles with `==`.
- Validation helpers (`utils::check_nonnegative` in `include/utils/utils.hpp`)
  throw `std::invalid_argument` / `std::out_of_range` — propagate those
  exception types from new boundary checks rather than inventing new ones.
