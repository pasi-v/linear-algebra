#ifndef PIVOT_INFO_HPP
#define PIVOT_INFO_HPP

#include "math_utils/math_utils.hpp"
#include "matrix.hpp"
#include <cstddef>
#include <vector>

namespace la {
inline bool is_zero_pivot(double x) { return math_utils::is_near_zero(x); }

struct PivotInfo {
    std::vector<std::size_t> pivot_cols;
    std::vector<std::size_t> free_cols;
};

PivotInfo find_pivots_and_free_cols(const Matrix &R);
} // namespace la

#endif // PIVOT_INFO_HPP
