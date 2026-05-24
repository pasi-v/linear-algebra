#include "doctest/doctest.h"
#include "la/matrix.hpp"
#include "test_utils.hpp"
#include <vector>

TEST_CASE("from_cols") {
    using la::from_cols;
    using la::Matrix;
    using la::Vector;

    SUBCASE("3x2 matrix") {
        Vector col1{1, 0, 3};
        Vector col2{-1, 1, -3};
        std::vector<Vector> cols{col1, col2};
        // clang-format off
        Matrix expected(3, 2, {
                                  1, -1,
                                  0,  1,
                                  3, -3
                              });
        // clang-format on
        CHECK_NEAR(expected, from_cols(cols));
    }

    SUBCASE("different size vectors throw") {
        Vector col1{1, 0, 3};
        Vector col2{-1, 1};
        std::vector<Vector> cols{col1, col2};
        CHECK_THROWS_AS(from_cols(cols), std::invalid_argument);
    }

    SUBCASE("no rows creates 0x0 matrix") {
        std::vector<Vector> cols;
        Matrix expected(0, 0);
        CHECK_NEAR(expected, from_cols(cols));
    }

    SUBCASE("m 0-length vectors creates mx0 matrix") {
        Vector col1(0);
        Vector col2(0);
        std::vector<Vector> cols{col1, col2};
        Matrix expected(0, 2);
        CHECK_NEAR(expected, from_cols(cols));
    }
}

TEST_CASE("flatten") {
    using la::flatten;
    using la::Matrix;
    using la::Vector;

    SUBCASE("2x2 matrix") {
        Matrix A(2, 2, {0, 1, -1, 0});
        Vector expected{0, 1, -1, 0}; // by rows, not cols
        CHECK_EQ(expected, flatten(A));
    }

    SUBCASE("1x3 matrix") {
        Matrix A(1, 3, {1, 2, 3});
        Vector expected{1, 2, 3};
        CHECK_EQ(expected, flatten(A));
    }

    SUBCASE("3x1 matrix") {
        Matrix A(3, 1, {1, 2, 3});
        Vector expected{1, 2, 3};
        CHECK_EQ(expected, flatten(A));
    }

    SUBCASE("0x0 matrix") {
        Matrix A(0, 0);
        Vector expected(0);
        CHECK_EQ(expected, flatten(A));
    }
}
