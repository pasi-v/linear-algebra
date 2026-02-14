#include "doctest/doctest.h"
#include "la/vector.hpp"
#include "la/vector_algorithms.hpp"

TEST_CASE("Dot product happy case") {
    using la::Vector;
    Vector u{1, 2, -3};
    Vector v{-3, 5, 2};
    double result = dot(u, v);
    CHECK(result == 1.0);
}

TEST_CASE("Dot product different sizes throws") {
    using la::Vector;
    Vector u(2);
    Vector v(3);
    CHECK_THROWS_AS(dot(u, v), std::invalid_argument);
}

TEST_CASE("Vector length") {
    using la::Vector;
    Vector u{2, 3};
    CHECK(norm(u) == std::sqrt(13.0));
}

TEST_CASE("Vector difference happy path") {
    using la::Vector;
    Vector u{std::sqrt(2), 1, -1};
    Vector v{0, 2, -2};
    CHECK(distance(u, v) == 2.0);
}

TEST_CASE("Difference different sizes throws") {
    using la::Vector;
    Vector u(2);
    Vector v(3);
    CHECK_THROWS_AS(distance(u, v), std::invalid_argument);
}

TEST_CASE("Angle happy path") {
    using la::Vector;
    Vector u{2, 1, -2};
    Vector v{1, 1, 1};
    double result = angle(u, v);
    CHECK(result == doctest::Approx(1.377).epsilon(0.01));
}

TEST_CASE("Angle different sizes throws") {
    using la::Vector;
    Vector u{1, 2};
    Vector v{1, 2, 3};
    CHECK_THROWS_AS(angle(u, v), std::invalid_argument);
}

TEST_CASE("Angle with zero vector throws") {
    using la::Vector;
    Vector u{0, 0, 0};
    Vector v{1, 2, 3};
    CHECK_THROWS_AS(angle(u, v), std::domain_error);
}

TEST_CASE("Projection happy path") {
    using la::Vector;
    Vector u{2, 1};
    Vector v{-1, 3};
    Vector result = proj_onto(u, v);
    Vector expected{2.0 / 5.0, 1.0 / 5.0};
    CHECK(result == expected);
}

TEST_CASE("Projection different sizes throws") {
    using la::Vector;
    Vector u(2);
    Vector v(3);
    CHECK_THROWS_AS(proj_onto(u, v), std::invalid_argument);
}
