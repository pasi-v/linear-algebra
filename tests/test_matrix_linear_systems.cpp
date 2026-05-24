#include "doctest/doctest.h"
#include "la/matrix.hpp"
#include "la/matrix_linear_systems.hpp"
#include "la/vector.hpp"

TEST_CASE("augment") {
    using la::Matrix;
    using la::Vector;

    // clang-format off
    Matrix A(3, 3, {
                       1, -1,  2,
                       1,  2, -1,
                       0,  2, -2
                   });

    SUBCASE("happy path") {
        Vector b({3, -3, 1});
        Matrix R(3, 4, {
                           1, -1,  2,  3,
                           1,  2, -1, -3,
                           0,  2, -2, 1
                       });
        // clang-format on

        CHECK_EQ(augment(A, b), R);
    }

    SUBCASE("wrong size b throws") {
        Vector b({1, 2});
        CHECK_THROWS_AS(augment(A, b), std::domain_error);
    }
}

TEST_CASE("is_in_span") {
    using la::Vector;

    SUBCASE("vectors and vector") {
        using la::is_in_span;
        Vector v1{1, 0, 3};
        Vector v2{-1, 1, -3};
        std::vector<Vector> vectors{v1, v2};

        SUBCASE("[1, 2, 3] is in span of [[1, 0, 3], [-1, 1, -3]]") {
            Vector b{1, 2, 3};
            CHECK(is_in_span(vectors, b));
        }

        SUBCASE("[2, 3, 4] is not in span of [[1, 0, 3], [-1, 1, -3]]") {
            Vector b{2, 3, 4};
            CHECK(!is_in_span(vectors, b));
        }

        SUBCASE("non-matching size of b throws") {
            Vector b{2, 3};
            CHECK_THROWS_AS(is_in_span(vectors, b), std::invalid_argument);
        }
    }

    SUBCASE("matrix and vector") {
        // clang-format off
        la::Matrix A(3, 2, {
           1, -1,
           0,  1,
           3, -3
        });
        // clang-format on

        SUBCASE("[1, 2, 3] is in span of [[1, -1], [0, 1], [3, -3]]") {
            Vector b{1, 2, 3};
            CHECK(is_in_span(A, b));
        }

        SUBCASE("[2, 3, 4] is not in span of [[1, -1], [0, 1], [3, -3]]") {
            Vector b{2, 3, 4};
            CHECK(!is_in_span(A, b));
        }

        SUBCASE("non-matching size of b throws") {
            Vector b{2, 3};
            CHECK_THROWS_AS(is_in_span(A, b), std::invalid_argument);
        }
    }
}

TEST_CASE("is_linear_combination") {
    using la::Matrix;

    Matrix A1(2, 2, {0, 1, -1, 0});
    Matrix A2(2, 2, {1, 0, 0, 1});
    Matrix A3(2, 2, {1, 1, 1, 1});
    std::vector<Matrix> matrices{A1, A2, A3};

    SUBCASE("true linear combination") {
        // Poole example 3.16 a), p. 153
        Matrix B(2, 2, {1, 4, 2, 1});
        CHECK(is_linear_combination(B, matrices));
    }

    SUBCASE("false linear combination") {
        // Poole example 3.16 b), p. 153
        Matrix B(2, 2, {1, 2, 3, 4});
        CHECK(!is_linear_combination(B, matrices));
    }

    SUBCASE("Matrix sizes don't match") {
        Matrix three_by_three(3, 3);
        std::vector<Matrix> matrices{A1, three_by_three, A3};
        CHECK_THROWS_AS(is_linear_combination(A2, matrices),
                        std::invalid_argument);
    }

    SUBCASE("empty matrices") {
        std::vector<Matrix> empty;
        CHECK_THROWS_AS(is_linear_combination(A1, empty),
                        std::invalid_argument);
    }
}
