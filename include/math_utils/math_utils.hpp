// math_utils.h
#ifndef MATH_UTILS_H
#define MATH_UTILS_H


#include <cmath>
namespace math_utils {

/**
 * @brief Numerical tolerance used for floating-point comparisons.
 *
 * Values with absolute magnitude less than or equal to `kEps` are
 * considered to be zero in equality checks and elimination steps.
 *
 * This constant defines the default threshold for detecting pivots,
 * zeroing entries, and other floating-point comparisons in matrix
 * operations such as REF/RREF transformations.
 *
 * @note Users may rely on this value when implementing custom
 *       algorithms to maintain consistency with the library’s
 *       numerical behavior.
 */
static constexpr double kEps = 1e-12;

inline bool is_zero(double x) { return std::abs(x) < kEps; }

/** @return the radians in degrees */
inline double toDegrees(double radians) { return radians * (180.0 / M_PI); }

/** @return the degrees in radians */
inline double toRadians(double degrees) { return degrees * (M_PI / 180.0); }

} // namespace math_utils

#endif // MATH_UTILS_H
