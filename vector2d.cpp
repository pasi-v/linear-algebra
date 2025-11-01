#include "doctest.h"
#include "la/math_utils.hpp"
#include <cmath>

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
  double directionDeg() const { return math_utils::toDegrees(directionRad()); }

  /** @return the direction of the vector in degrees in [0, 360] */
  double directionDeg0To360() const {
    return math_utils::toDegrees(directionRad0To2Pi());
  }

private:
  double x_;
  double y_;
};

double Vector2D::directionRad0To2Pi() const {
  double angle = directionRad();
  return angle < 0 ? angle + 2.0 * M_PI : angle;
}

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

TEST_CASE("Construct vector") {
  Vector2D v = {1, 2};
  CHECK_EQ(v.x(), 1);
  CHECK_EQ(v.y(), 2);
}

TEST_CASE("Length") {
  Vector2D v = {3, 4};
  CHECK_EQ(v.length(), 5.0);
}

TEST_CASE("Multiply by scalar") {
  Vector2D v = {-1, 2};
  Vector2D expected = {-3, 6};
  CHECK_EQ(v * 3, expected);
}

TEST_CASE("Operator +") {
  Vector2D a = {2, 3};
  Vector2D b = {5, -1};
  Vector2D expected = {7, 2};
  CHECK_EQ(a + b, expected);
}

TEST_CASE("Operator -") {
  Vector2D a = {2, 3};
  Vector2D b = {5, -1};
  Vector2D expected = {-3, 4};
  CHECK_EQ(a - b, expected);
}

TEST_CASE("direction") {
  Vector2D v = {3, 4};
  CHECK(v.directionRad() == doctest::Approx(0.93).epsilon(0.01));
  CHECK(v.directionRad0To2Pi() == doctest::Approx(0.93).epsilon(0.01));
}

TEST_CASE("direction in quadrant 4") {
  Vector2D v = {3, -4};
  CHECK(v.directionRad() == doctest::Approx(-0.93).epsilon(0.01));
  CHECK(v.directionRad0To2Pi() == doctest::Approx(5.36).epsilon(0.01));
}

TEST_CASE("direction in degrees") {
  Vector2D v = {3, 4};
  CHECK(v.directionDeg() == doctest::Approx(53.0).epsilon(0.01));
  CHECK(v.directionDeg0To360() == doctest::Approx(53.0).epsilon(0.01));
}

TEST_CASE("direction in degrees in quadrant 4") {
  Vector2D v = {3, -4};
  CHECK(v.directionDeg0To360() == doctest::Approx(307.0).epsilon(0.01));
}

TEST_CASE("fromPolar along x axis") {
  Vector2D v = Vector2D::fromPolar(0.0, 1.0);
  CHECK(v == Vector2D(1, 0));
}

TEST_CASE("fromPolar in quadrant 2") {
  Vector2D v = Vector2D::fromPolar(math_utils::toRadians(120), 2);
  CHECK(v.x() == doctest::Approx(-1.0).epsilon(0.01));
  CHECK(v.y() == doctest::Approx(1.73).epsilon(0.01));
}

TEST_CASE("sum from magnitudes and angles in quadrant 4") {
  // This is the way these are often presented in maths problems:
  Vector2D v = Vector2D::fromPolar(math_utils::toRadians(20), 8);
  Vector2D w = Vector2D::fromPolar(math_utils::toRadians(240), 5);
  Vector2D sum = v + w;
  CHECK(sum.directionDeg0To360() == doctest::Approx(342.4).epsilon(0.1));
  CHECK(sum.length() == doctest::Approx(4.8).epsilon(0.1));
}

TEST_CASE("DirectedVector direction and length") {
  DirectedVector2D dv = {{2, 3}, {5, -1}};
  Vector2D direction = dv.direction();
  Vector2D expected = {3, -4};
  CHECK_EQ(direction, expected);
  CHECK_EQ(dv.length(), 5);
}
