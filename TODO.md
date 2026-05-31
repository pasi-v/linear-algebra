# TODO

## Functionality

### Application

- [ ] solve a linear system `[A|b]`
- [ ] take libedit into use


## Refactoring

### Matrix::ref implementation details

From ChatGPT review of ref (https://chatgpt.com/c/68c54ad2-57d4-8333-b14e-4a18d20b48fa).
Items 3 and 10 have landed (no row/col copies in `ref()`; inner loop starts at
`lead_col`; `m`/`n` cached). Remaining:

- [ ] **Make integer/exact math configurable.** Template the epsilon behavior
  so ℚ/ℤ types can use strict `== 0` while floating point uses `kEps`.
- [ ] **Remove the early `is_ref()` short-circuit in `ref()`**
  (`src/row_reduction.cpp:174`). Optional — the main loop exits quickly
  anyway.
- [ ] **Handle degenerate shapes up front.** Add
  `if (R.rows() == 0 || R.cols() == 0) return R;` at the top of `ref()` to
  make invariants explicit.
- [ ] **Finish the int↔size_t cleanup.** `ref()` is `size_t` end-to-end, but
  `first_non_zero_column` still returns `int` (`include/la/vector_algorithms.hpp:45`)
  and `is_ref` still keeps an `int prev_leading_entry_column = -1` sentinel
  (`src/row_reduction.cpp:122`). Switch the API to `std::optional<std::size_t>`
  (or `std::ptrdiff_t` if the sentinel is wanted) and propagate.
- [ ] **Return metadata from `ref`.** Add a `RefResult { Matrix R;
  std::vector<std::size_t> pivot_cols; std::size_t rank; }` variant so later
  solves/tests don't have to re-scan for pivot columns.
- [ ] **RowView/ColView (optional ergonomics).** Add a lightweight non-owning
  row/column span so users of `Matrix` can iterate without `Vector` copies.
  Would let `is_ref`/`is_rref` drop their `A.row(i)` copies too.


## Minor cleanups

- [ ] `app/evaluator.cpp:198` — `spanning_vectors` is appended in a loop over
  `spanning_names` (known size) but not reserved. Add
  `spanning_vectors.reserve(spanning_names.size());` before the loop to avoid
  geometric reallocations of the underlying buffer. Matches the
  reserve+push_back pattern used elsewhere (e.g. `pivot_info.cpp`,
  `linear_system.cpp:44`).


## Separating core structures and algebra functions

Substantially done: `ref`/`rref`/`rank`, `det`, transforms, linear-system
solvers, and the vector algorithms (`dot`/`norm`/`angle`/`proj_onto`/...) all
live as free functions in dedicated headers
(`row_reduction.hpp`, `determinant.hpp`, `matrix_transforms.hpp`,
`matrix_linear_systems.hpp`, `vector_algorithms.hpp`).
See https://chatgpt.com/c/68c54ad2-57d4-8333-b14e-4a18d20b48fa for the
original suggestions. Read Effective C++ for more.

Remaining:

- [ ] Move `Matrix::operator+`, `operator-`, `operator*(double)`,
  `operator*(const Matrix&)`, `operator*(const Vector&)` out of the class
  into free functions (`src/matrix.cpp:46-116`).
- [ ] Move `Vector::operator+`, `operator-`, `operator*(double)` out of the
  class. Decide whether `subvector` / `head` / `tail` count as "core" or
  belong with the algorithms (`include/la/vector.hpp:80-115`).
