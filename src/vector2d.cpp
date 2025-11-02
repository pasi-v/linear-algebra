#include "la/vector2d.hpp"
#include "math_utils/math_utils.hpp"
#include <cmath>

namespace la {
double Vector2D::directionRad0To2Pi() const {
    double angle = directionRad();
    return angle < 0 ? angle + 2.0 * M_PI : angle;
}
} // namespace la
