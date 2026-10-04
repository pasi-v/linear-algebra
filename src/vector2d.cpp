#include "la/vector2d.hpp"
#include "la/matrix.hpp"
#include "math_utils/math_utils.hpp"
#include <cmath>

namespace la {

Vector to_vector(const Vector2D &v); // {v.x(), v.y()}
Vector2D
to_vector2d(const Vector &v); // throws std::invalid_argument if v.size() != 2

double Vector2D::directionRad0To2Pi() const {
    double angle = directionRad();
    return angle < 0 ? angle + 2.0 * M_PI : angle;
}

Vector2D rotate(const Vector2D &v, double theta) {
    double cosTheta = cos(math_utils::toRadians(theta));
    double sinTheta = sin(math_utils::toRadians(theta));
    Matrix R(2, 2, {cosTheta, -sinTheta, sinTheta, cosTheta});
    return to_vector2d(R * to_vector(v));
}

Vector to_vector(const Vector2D &v) { return Vector({v.x(), v.y()}); }

Vector2D to_vector2d(const Vector &v) {
    if (v.size() != 2)
        throw std::invalid_argument("v must have size 2");
    return Vector2D(v[0], v[1]);
}

} // namespace la
