#include "doctest.h"
#include "la/vector3d.hpp"
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

constexpr double EPSILON = 1e-9;

/**
 * Plane equation in standard form ax + by + cz = d
 */
struct PlaneEquation {
  double a, b, c, d; // represents ax + by + cz = d

  bool operator==(const PlaneEquation &other) const {
    return std::fabs(a - other.a) < EPSILON &&
           std::fabs(b - other.b) < EPSILON &&
           std::fabs(c - other.c) < EPSILON &&
           std::fabs(d - other.d) < EPSILON;
  }

  Vector3D normal() const { return Vector3D{a, b, c}; }

  std::string to_string() const {
    std::ostringstream out;
    out << a << "x + " << b << "y + " << c << "z = " << d;
    return out.str();
  }
};

/**
 * Plane in 3D
 */
class Plane3D {
public:
  /**
   * Construct a Plane from a point on the plane and two vectors on the plane.
   *
   * @param p the point on the plane
   * @param u a vector on the plane
   * @param v a vector on the plane
   *
   * @return a Plane
   */
  Plane3D(const Vector3D p, const Vector3D u, const Vector3D v)
      : p_(p), u_(u), v_(v) {}

  static Plane3D from_points(const Vector3D p, const Vector3D q,
                             const Vector3D r) {
    Vector3D u = q - p;
    Vector3D v = r - p;
    return Plane3D(p, u, v);
  }

  PlaneEquation to_standard_form() const;

private:
  Vector3D p_; // Point on the plane
  Vector3D u_; // Vector on the plane
  Vector3D v_; // Vector on the plane
};

PlaneEquation Plane3D::to_standard_form() const {
  Vector3D n = u_.crossProduct(v_);
  double d = n.dotProduct(p_);

  return {n.x(), n.y(), n.z(), d};
}

TEST_CASE("Plane equation") {
  Vector3D p = {1, 0, -2};
  Vector3D u = {0, 1, 1};
  Vector3D v = {3, -1, 2};
  Plane3D plane = {p, u, v};
  PlaneEquation actual = plane.to_standard_form();
  // We don't do simplification by gcd or normalization yet.
  PlaneEquation expected = {3, 3, -3, 9};
  CHECK(actual == expected);
}

TEST_CASE("Plane equation from points") {
  Vector3D p = {0, -1, 1};
  Vector3D q = {2, 0, 2};
  Vector3D r = {1, 2, -1};
  Plane3D plane = Plane3D::from_points(p, q, r);
  PlaneEquation actual = plane.to_standard_form();
  PlaneEquation expected = {-5, 5, 5, 0};
  CHECK(actual == expected);
}
