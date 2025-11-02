#ifndef VECTOR_2D_HPP
#define VECTOR_2D_HPP

#include "math_utils/math_utils.hpp"
#include <cmath>

namespace la {
/**
 * A class for representing a vector in geometric plane (R2).
 */
class Vector2D {
  public:
    /** @return a vector from x and y components */
    Vector2D(const double x, const double y) : x_(x), y_(y) {}

    /**
     * @brief construct a vector from polar coordinates
     * @param angleRadians the angle in [-pi, pi]
     * @param magnitude the length of the vector
     * @return a vector
     */
    static Vector2D fromPolar(double angleRadians, double magnitude) {
        double x = magnitude * std::cos(angleRadians);
        double y = magnitude * std::sin(angleRadians);
        return Vector2D(x, y);
    }

    /** @return the x component */
    double x() const { return x_; }

    /** @return the y component */
    double y() const { return y_; }

    /** @return true if the x and y components are equal */
    friend bool operator==(const Vector2D a, const Vector2D b) {
        return a.x() == b.x() && a.y() == b.y();
    }

    /** @return the vector multiplied by scalar c */
    Vector2D operator*(double c) const { return Vector2D(x_ * c, y_ * c); }

    /** @return the sum of vectors */
    Vector2D operator+(const Vector2D v) const {
        return Vector2D(x_ + v.x(), y_ + v.y());
    }

    /** @return the difference of vectors */
    Vector2D operator-(const Vector2D v) const {
        return Vector2D(x_ - v.x(), y_ - v.y());
    }

    /** @return the length (magnitude) of the vector */
    double length() const { return std::sqrt(x_ * x_ + y_ * y_); }

    /** @return the direction of the vector in radians in [-pi, pi] */
    double directionRad() const { return atan2(y_, x_); }

    /** @return the direction of the vector in radians in [0, 2pi] */
    double directionRad0To2Pi() const;

    /** @return the direction of the vector in degrees in [-180, 180] */
    double directionDeg() const {
        return math_utils::toDegrees(directionRad());
    }

    /** @return the direction of the vector in degrees in [0, 360] */
    double directionDeg0To360() const {
        return math_utils::toDegrees(directionRad0To2Pi());
    }

  private:
    double x_;
    double y_;
};

/**
 * Struct for representing a point in 2D geometric plane
 */
struct Point2D {
    double x, y;
};

/**
 * Struct for representing a directed vector from start point to end point in
 * 2D plane
 */
struct DirectedVector2D {
    Point2D start;
    Point2D end;

    /** @return a Vector2D direction vector from this struct */
    Vector2D direction() const {
        return Vector2D{end.x - start.x, end.y - start.y};
    }

    /** @return the distance between start and end points */
    double length() const {
        auto d = direction();
        return std::sqrt(d.x() * d.x() + d.y() * d.y());
    }
};

} // namespace la

#endif
