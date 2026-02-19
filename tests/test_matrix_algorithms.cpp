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

TEST_CASE("transpose") {
    using la::Matrix;

    SUBCASE("2x3 matrix transposes correctly to 3x2 matrix") {
        // clang-format off
        Matrix A(2, 3, {
            1, 3, 2,
            5, 0, 1
        });
        Matrix expected(3, 2, {
            1, 5,
            3, 0,
            2, 1
        });
        // clang-format on
        CHECK_EQ(transpose(A), expected);
    }

    SUBCASE("1x3 'vector' transposes correctly to 3x1 'vector'") {
        Matrix A(1, 3, {5, -1, 2});
        Matrix expected(3, 1, {5, -1, 2});
        CHECK_EQ(transpose(A), expected);
    }
}

TEST_CASE("is_symmetric") {
    using la::Matrix;

    SUBCASE("symmetric matrix") {
        // clang-format off
        Matrix A(3, 3, {
            1, 3, 2,
            3, 5, 0,
            2, 0, 4
        });
        // clang-format on
        CHECK(is_symmetric(A));
    }

    SUBCASE("asymmetric square matrix is not symmetric") {
        // clang-format off
        Matrix A(2, 2, {
             1, 2,
            -1, 3
        });
        // clang-format on
        CHECK(!is_symmetric(A));
    }

    SUBCASE("non-square matrix is not symmetric") {
        Matrix A(1, 2, {3, 4});
        CHECK(!is_symmetric(A));
    }

    SUBCASE("1-row 1-col matrix is symmetric") {
        Matrix A(1, 1);
        CHECK(is_symmetric(A));
    }
}
