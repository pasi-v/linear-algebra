// math_utils.h
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cmath> // for M_PI

namespace math_utils {

inline double toDegrees(double radians) { return radians * (180.0 / M_PI); }

inline double toRadians(double degrees) { return degrees * (M_PI / 180.0); }

} // namespace math_utils

#endif // MATH_UTILS_H
