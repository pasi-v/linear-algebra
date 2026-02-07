#include "la/pivot_info.hpp"
#include "la/matrix_algorithms.hpp"

namespace la {
// R is in REF and pivots are ordered from top left to bottom right.
PivotInfo find_pivots_and_free_cols(const Matrix &R) {
    const std::size_t n = R.cols() - 1; // number of variables
    const std::size_t r = rank(R);      // rank(A) == number of pivot rows

    PivotInfo info;
    info.pivot_cols.reserve(r);
    info.free_cols.reserve(n - r);

    std::size_t pivot_row = 0;

    for (std::size_t col = 0; col < n; ++col) {
        // We are scanning left-to-right; for each variable column,
        // either this column is the pivot for pivot_row, or it’s free.
        if (pivot_row < r && !is_zero_pivot(R(pivot_row, col))) {
            // This column has the pivot for this row
            info.pivot_cols.push_back(col);
            ++pivot_row;
        } else {
            // No pivot here -> free variable column
            info.free_cols.push_back(col);
        }
    }

    return info;
}
} // namespace la
