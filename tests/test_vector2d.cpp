#include "doctest/doctest.h"
#include "la/vector2d.hpp"

TEST_CASE("Construct vector") {
    using la::Vector2D;
    Vector2D v = {1, 2};
    CHECK_EQ(v.x(), 1);
    CHECK_EQ(v.y(), 2);
}

TEST_CASE("Length") {
    using la::Vector2D;
    Vector2D v = {3, 4};
    CHECK_EQ(v.length(), 5.0);
}

TEST_CASE("Multiply by scalar") {
    using la::Vector2D;
    Vector2D v = {-1, 2};
    Vector2D expected = {-3, 6};
    CHECK_EQ(v * 3, expected);
}

TEST_CASE("Operator +") {
    using la::Vector2D;
    Vector2D a = {2, 3};
    Vector2D b = {5, -1};
    Vector2D expected = {7, 2};
    CHECK_EQ(a + b, expected);
}

TEST_CASE("Operator -") {
    using la::Vector2D;
    Vector2D a = {2, 3};
    Vector2D b = {5, -1};
    Vector2D expected = {-3, 4};
    CHECK_EQ(a - b, expected);
}

TEST_CASE("direction") {
    using la::Vector2D;
    Vector2D v = {3, 4};
    CHECK(v.directionRad() == doctest::Approx(0.93).epsilon(0.01));
    CHECK(v.directionRad0To2Pi() == doctest::Approx(0.93).epsilon(0.01));
}

TEST_CASE("direction in quadrant 4") {
    using la::Vector2D;
    Vector2D v = {3, -4};
    CHECK(v.directionRad() == doctest::Approx(-0.93).epsilon(0.01));
    CHECK(v.directionRad0To2Pi() == doctest::Approx(5.36).epsilon(0.01));
}

TEST_CASE("direction in degrees") {
    using la::Vector2D;
    Vector2D v = {3, 4};
    CHECK(v.directionDeg() == doctest::Approx(53.0).epsilon(0.01));
    CHECK(v.directionDeg0To360() == doctest::Approx(53.0).epsilon(0.01));
}

TEST_CASE("direction in degrees in quadrant 4") {
    using la::Vector2D;
    Vector2D v = {3, -4};
    CHECK(v.directionDeg0To360() == doctest::Approx(307.0).epsilon(0.01));
}

TEST_CASE("fromPolar along x axis") {
    using la::Vector2D;
    Vector2D v = Vector2D::fromPolar(0.0, 1.0);
    CHECK(v == Vector2D(1, 0));
}

TEST_CASE("fromPolar in quadrant 2") {
    using la::Vector2D;
    Vector2D v = Vector2D::fromPolar(math_utils::toRadians(120), 2);
    CHECK(v.x() == doctest::Approx(-1.0).epsilon(0.01));
    CHECK(v.y() == doctest::Approx(1.73).epsilon(0.01));
}

TEST_CASE("sum from magnitudes and angles in quadrant 4") {
    using la::Vector2D;
    // This is the way these are often presented in maths problems:
    Vector2D v = Vector2D::fromPolar(math_utils::toRadians(20), 8);
    Vector2D w = Vector2D::fromPolar(math_utils::toRadians(240), 5);
    Vector2D sum = v + w;
    CHECK(sum.directionDeg0To360() == doctest::Approx(342.4).epsilon(0.1));
    CHECK(sum.length() == doctest::Approx(4.8).epsilon(0.1));
}

TEST_CASE("DirectedVector direction and length") {
    using la::DirectedVector2D;
    using la::Vector2D;

    DirectedVector2D dv = {{2, 3}, {5, -1}};
    Vector2D direction = dv.direction();
    Vector2D expected = {3, -4};
    CHECK_EQ(direction, expected);
    CHECK_EQ(dv.length(), 5);
}
