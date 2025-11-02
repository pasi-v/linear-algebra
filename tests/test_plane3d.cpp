#include "doctest/doctest.h"
#include "la/plane3d.hpp"

TEST_CASE("Plane equation") {
    using la::Plane3D;
    using la::PlaneEquation;
    using la::Vector3D;

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
    using la::Plane3D;
    using la::PlaneEquation;
    using la::Vector3D;

    Vector3D p = {0, -1, 1};
    Vector3D q = {2, 0, 2};
    Vector3D r = {1, 2, -1};
    Plane3D plane = Plane3D::from_points(p, q, r);
    PlaneEquation actual = plane.to_standard_form();
    PlaneEquation expected = {-5, 5, 5, 0};
    CHECK(actual == expected);
}
