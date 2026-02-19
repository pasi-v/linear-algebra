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

TEST_CASE("is_zero returns TRUE for zero vector") {
    using la::Vector;
    Vector v{0, 0, 0};
    CHECK(is_zero(v));
}

TEST_CASE("is_zero returns FALSE for non-zero element in vector") {
    using la::Vector;
    Vector v{0, 3, 0};
    CHECK(!is_zero(v));
}

TEST_CASE("Vector scalar multiplication happy path") {
    using la::Vector;
    int c = 2;
    Vector v{-2, 4};
    Vector cv = v * c;
    Vector expected{-4, 8};
    CHECK(cv == expected);
}

TEST_CASE("Vector scalar multiplication scalar first") {
    using la::Vector;
    int c = 2;
    Vector v{-2, 4};
    Vector cv = c * v;
    Vector expected{-4, 8};
    CHECK(cv == expected);
}

TEST_CASE("is_standard_basis returns true if one element is 1 and rest 0") {
    using la::Vector;
    Vector v{0, 1, 0};
    CHECK(is_standard_basis(v));
}

TEST_CASE("is_standard_basis returns false if two elements are 1 and rest 0") {
    using la::Vector;
    Vector v{0, 1, 1};
    CHECK(!is_standard_basis(v));
}

TEST_CASE("is_standard_basis returns false if one element is 2 and rest 0") {
    using la::Vector;
    Vector v{0, 2, 0};
    CHECK(!is_standard_basis(v));
}

TEST_CASE("is_standard_basis returns false for zero vector") {
    using la::Vector;
    Vector v{0, 0, 0};
    CHECK(!is_standard_basis(v));
}

TEST_CASE("first_non_zero_column returns 0 if first element is non-zero") {
    using la::Vector;
    Vector v{1, 2, 3};
    CHECK_EQ(0, first_non_zero_column(v));
}

TEST_CASE(
    "first_non_zero_column returns m-1 if only last element is non-zero") {
    using la::Vector;
    Vector v{0, 0, 1};
    CHECK_EQ(2, first_non_zero_column(v));
}

TEST_CASE("first_non_zero_column returns -1 for zero vector") {
    using la::Vector;
    Vector v{0, 0, 0};
    CHECK_EQ(-1, first_non_zero_column(v));
}

TEST_CASE("leading_element returns first element if it is not zero") {
    using la::Vector;
    Vector v{1, 2, 3};
    CHECK_EQ(1, leading_element(v));
}

TEST_CASE("leading_element returns last element if preceding ones are zero") {
    using la::Vector;
    Vector v{0, 0, 4};
    CHECK_EQ(4, leading_element(v));
}

TEST_CASE("leading_element returns 0 for zero vector") {
    using la::Vector;
    Vector v{0, 0, 0};
    CHECK_EQ(0, leading_element(v));
}

TEST_CASE("has_leading_one returns true if leading element is 1") {
    using la::Vector;
    Vector v{0, 1, 2};
    CHECK(has_leading_one(v));
}

TEST_CASE("has_leading_one returns false if leading element is 3") {
    using la::Vector;
    Vector v{0, 3, 0};
    CHECK(!has_leading_one(v));
}

TEST_CASE("has_leading_one returns false for zero vector") {
    using la::Vector;
    Vector v{0, 0, 0};
    CHECK(!has_leading_one(v));
}
