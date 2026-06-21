# TODO

## Functionality

### Application

- [ ] solve a linear system `[A|b]`
- [ ] take libedit into use


## Refactoring

### Matrix::ref implementation details

From ChatGPT review of ref (https://chatgpt.com/c/68c54ad2-57d4-8333-b14e-4a18d20b48fa).
Remaining:

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


### Configurable scalar type (integer/exact math)

Prerequisite for the "Make integer/exact math configurable" item above. Today
the library is `double` end to end (`Vector::value_type`, both `data_` buffers,
and every algorithm signature), so there is nothing for an epsilon policy to
switch on. Open question to settle first: **is exact arithmetic actually a
goal, or is this just keeping the door open?** It implies a library-wide
template refactor.

Two non-obvious constraints:

- Template the *element type* (`Vector<T>`/`Matrix<T>` with `using Vector =
  Vector<double>;`); don't add parallel `IntVector`/`IntMatrix` classes — that
  duplicates every free-function algorithm.
- `int` (ℤ) does **not** give exact `rref`/`solve`: Gaussian elimination
  divides by pivots and ℤ isn't closed under division. The scalar that buys
  exact row reduction is `Rational` (ℚ). Plain `int` only helps for the
  division-free subset (add/sub/mul, dot, a fraction-free Bareiss determinant).

Suggested ordering:

- [ ] Template `Vector`/`Matrix` on `T`, alias the current names to `<double>`
  (large, mechanical; keeps existing tests green).
- [ ] Add a `ScalarPolicy<T>` trait (`is_zero`, `is_one`); route `is_zero_pivot`
  (`include/la/pivot_policy.hpp`) and the direct comparison sites through it —
  `src/eliminated_system.cpp:19`, `src/row_reduction.cpp:156`,
  `src/matrix_transforms.cpp:39`. **This is the "configurable epsilon" item.**
- [ ] Add a `Rational` (and/or `int`) scalar type; specialize the policy for
  exact `==`.
- [ ] Revisit which algorithms are division-free vs. need a fraction-free
  variant for ℤ.


## Minor cleanups

- [ ] **Unify size-mismatch exceptions on `std::invalid_argument`.** The
  `CLAUDE.md` convention is now: dimensional non-conformance throws
  `std::invalid_argument`, `std::domain_error` is reserved for "well-formed
  input, operation mathematically undefined" (e.g. `angle` of a zero vector).
  These sites still throw/document `domain_error` for a shape mismatch and
  need re-pointing:
  - `src/matrix_linear_systems.cpp:11` — `augment(A, b)` throw site
    (`std::domain_error` → `std::invalid_argument`). This is the single
    runtime check; `eliminate_system`/`n_solutions`/`solve` all funnel
    through it.
  - `augment(A, B)` — verify its throw is `invalid_argument` (already added
    in the same file) and the doc matches.
  - Doc comments `@throws std::domain_error` → `@throws std::invalid_argument`:
    `include/la/matrix_linear_systems.hpp:16` (augment A,b),
    `:25` (augment A,B); `include/la/linear_system.hpp:36` (n_solutions),
    `:46` (solve); `include/la/eliminated_system.hpp:31` (eliminate_system).
  - Test: `tests/test_matrix_linear_systems.cpp:31` —
    `CHECK_THROWS_AS(augment(A, b), std::domain_error)` → `std::invalid_argument`.
  - Leave as-is (correctly `domain_error`): `angle` of a zero vector
    (`tests/test_vector_algorithms.cpp:60`). `determinant` non-square
    (`tests/test_determinant.cpp:14,20`) is a deferred judgment call — out of
    scope for this conformance sweep.
  - Done when `grep -rn domain_error include/la/matrix_linear_systems.hpp
    include/la/linear_system.hpp include/la/eliminated_system.hpp
    src/matrix_linear_systems.cpp` is empty and `make test` stays green.

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
