// math_utils.h
#ifndef MATH_UTILS_H
#define MATH_UTILS_H


#include <cmath>
#include <algorithm>

namespace math_utils {

constexpr double kDefaultAbsTol = 1e-12;
constexpr double kDefaultRelTol = 1e-10;

inline bool nearly_equal(double a, double b,
                         double abs_tol,
                         double rel_tol)
{
    return std::fabs(a - b)
    <= abs_tol + rel_tol * std::max(std::fabs(a), std::fabs(b));
}

inline bool nearly_equal(double a, double b)
{
    return nearly_equal(a, b, kDefaultAbsTol, kDefaultRelTol);
}

inline bool is_near_zero(double x) { return nearly_equal(x, 0.0);}

/** @return the radians in degrees */
inline double toDegrees(double radians) { return radians * (180.0 / M_PI); }

/** @return the degrees in radians */
inline double toRadians(double degrees) { return degrees * (M_PI / 180.0); }

} // namespace math_utils

#endif // MATH_UTILS_H
