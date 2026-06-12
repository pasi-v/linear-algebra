#ifndef PIVOT_POLICY_HPP
#define PIVOT_POLICY_HPP

#include "math_utils/math_utils.hpp"

namespace la {

constexpr double kPivotAbsTol = 1e-12;

inline bool is_zero_pivot(double x) {
    return math_utils::is_near_zero_abs(x, kPivotAbsTol);
}

} // namespace la

#endif // PIVOT_POLICY_HPP
