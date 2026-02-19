#include "doctest/doctest.h"
#include "la/matrix.hpp"
#include "la/matrix_algorithms.hpp"
#include "la/pivot_info.hpp"
#include "test_utils.hpp"

TEST_CASE("2x2 determinant happy path") {
    using la::Matrix;
    Matrix m(2, 2, {1, 2, 3, 4});
    CHECK_EQ(determinant(m), -2);
};

TEST_CASE("determinant of non-square matrix throws") {
    using la::Matrix;
    Matrix m(2, 3);
    CHECK_THROWS_AS(determinant(m), std::domain_error);
}

TEST_CASE("determinant of larger than 3x3 matrix throws") {
    using la::Matrix;
    Matrix m(4, 4);
    CHECK_THROWS_AS(determinant(m), std::domain_error);
}

TEST_CASE("determinant of 1x1 matrix is its only value") {
    using la::Matrix;
    Matrix m(1, 1, 3);
    CHECK_EQ(determinant(m), 3);
}

TEST_CASE("3x3 determinant happy path") {
    using la::Matrix;
    Matrix m(3, 3, {5, -3, 2, 1, 0, 2, 2, -1, 3});
    CHECK_EQ(determinant(m), 5);
}
