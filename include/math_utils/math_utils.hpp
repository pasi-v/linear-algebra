// math_utils.h
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cmath> // for M_PI

namespace math_utils {

/** @return the radians in degrees */
inline double toDegrees(double radians) { return radians * (180.0 / M_PI); }

/** @return the degrees in radians */
inline double toRadians(double degrees) { return degrees * (M_PI / 180.0); }

} // namespace math_utils

#endif // MATH_UTILS_H
