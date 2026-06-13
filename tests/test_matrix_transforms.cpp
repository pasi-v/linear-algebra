#include "doctest/doctest.h"
#include "la/matrix.hpp"
#include "la/matrix_transforms.hpp"

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

TEST_CASE("inverse") {
    using la::inverse;
    using la::Matrix;

    SUBCASE("Non-square matrix") {
        Matrix non_square(2, 3);
        Matrix out;
        CHECK_THROWS_AS(inverse(non_square, out), std::invalid_argument);
    }

    SUBCASE("Invertible matrix") {
        // clang-format off
        Matrix invertible(2, 2, {
            -1,  1.5,
             1, -1
        });
        Matrix expected(2, 2, {
            2, 3,
            2, 2
        });
        // clang-format on
        Matrix out;
        bool was_invertible = inverse(invertible, out);
        CHECK(was_invertible);
        CHECK_EQ(out, expected);
    }

    SUBCASE("Non-invertible matrix") {
        // clang-format off
        Matrix non_invertible(2, 2, {
            2, 4,
            2, 4
        });
        // clang-format off
        Matrix out;
        bool was_invertible = inverse(non_invertible, out);
        CHECK(!was_invertible);
    }
}
