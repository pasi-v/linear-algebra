#ifndef PIVOT_POLICY_HPP
#define PIVOT_POLICY_HPP

#include "math_utils/math_utils.hpp"

namespace la {

constexpr double kPivotAbsTol = 1e-12;
constexpr double kPivotRelTol = 1e-10;

inline bool is_zero_pivot(double x) {
    return math_utils::is_near_zero_abs(x, kPivotAbsTol);
}

inline bool is_effectively_zero_pivot(double x, double scale) {
    return std::fabs(x) <= kPivotAbsTol + kPivotRelTol * scale;
}

} // namespace la

#endif // PIVOT_POLICY_HPP
