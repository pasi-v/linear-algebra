# TODO

## Matrix::ref implementation details

From ChatGPT review of ref (https://chatgpt.com/c/68c54ad2-57d4-8333-b14e-4a18d20b48fa):

3. Stop copying rows/columns
Your current code creates Vector top_row, scaled_row, current, etc. That’s extra heap/loop work. Operate directly on R(i,j) with tight inner loops. If you want ergonomics, implement a lightweight, non-owning RowView/ColView (span-like) that doesn’t copy.

4. Tolerances instead of == 0.0
Never compare to 0.0 in elimination. Use kEps. That also applies to:
pivot detection (fabs(...) > kEps)
deciding whether to eliminate (fabs(factor) > kEps)
is_ref() and any tests

5. Make integer/exact math configurable
If you’ll sometimes work over ℚ/ℤ, you can template the epsilon behavior:
For floating point, use kEps.
For exact types, use strict == 0.

6. Remove the early is_ref() (optional)
It’s fine, but usually unnecessary; the main loop exits quickly if the matrix is already in REF. If is_ref() is cheap and well-tested, keeping it is okay.

7. Handle degenerate shapes up front
Short-circuit empty matrices or zero-column cases; makes invariants explicit.
if (R.rows_ == 0 || R.cols_ == 0) return R;

8. Avoid int↔size_t churn
Keep indices as std::size_t end-to-end to avoid casts. If helper APIs (like first_non_zero_column()) return int, change them to std::ptrdiff_t (for -1 sentinel) or return an optional<size_t>.
Example:
std::optional<std::size_t>
first_nonzero_in_col(const Matrix& A, std::size_t col, std::size_t from_row);

9. Optional: return metadata
For later solves/tests, returning pivot columns is handy:
struct RefResult { Matrix R; std::vector<std::size_t> pivot_cols; int rank; };
RefResult Matrix::ref_with_meta() const;
You can fill pivot_cols inside the loop when you find lead_col.

10. Micro-optimizations (when you care)
Start the inner elimination loop at lead_col (as above) to skip guaranteed zeros to the left.
Cache m, n outside loops.

If rows are contiguous, consider std::transform or simple pointer arithmetic for tight loops.


## Separating Matrix core structures and algebra functions

Move matrix algebra out of Matrix class. Only leave core structures like row and column manipulation
in the Matrix class.  See https://chatgpt.com/c/68c54ad2-57d4-8333-b14e-4a18d20b48fa for suggestions.

Read Effective C++ for more.


## Separating Vector core and algebra

Like Matrix above.
