#ifndef PIVOT_INFO_HPP
#define PIVOT_INFO_HPP

#include "matrix.hpp"
#include <cstddef>
#include <vector>

namespace la {
/**
 * @brief Numerical tolerance used for pivot comparisons.
 *
 * This constant defines the default threshold for detecting pivots,
 * zeroing entries, and other floating-point comparisons in matrix
 * operations such as REF/RREF transformations.
 *
 * @note Users may rely on this value when implementing custom
 *       algorithms to maintain consistency with the library’s
 *       numerical behavior.
 */
static constexpr double kPivotEps = 1e-12;

inline bool is_zero_pivot(double x) { return std::abs(x) < kPivotEps; }

struct PivotInfo {
    std::vector<std::size_t> pivot_cols;
    std::vector<std::size_t> free_cols;
};

PivotInfo find_pivots_and_free_cols(const Matrix &R);
} // namespace la

#endif // PIVOT_INFO_HPP
