#include "doctest/doctest.h"
#include "la/matrix.hpp"
#include "la/matrix_linear_systems.hpp"
#include "la/vector.hpp"

TEST_SUITE("augment") {
    using la::Matrix;

    // clang-format off
    Matrix A(3, 3, {
        1, -1,  2,
        1,  2, -1,
        0,  2, -2
    });
    // clang-format on

    TEST_CASE("augment with vector") {
        using la::Vector;

        SUBCASE("happy path") {
            Vector b({3, -3, 1});
            // clang-format off
            Matrix R(3, 4, {
                1, -1,  2,  3,
                1,  2, -1, -3,
                0,  2, -2,  1
            });
            // clang-format on

            CHECK_EQ(augment(A, b), R);
        }

        SUBCASE("wrong size b throws") {
            Vector b({1, 2});
            CHECK_THROWS_AS(augment(A, b), std::invalid_argument);
        }
    }

    TEST_CASE("augment with Matrix") {
        SUBCASE("wrong size B throws") {
            Matrix B(2, 3);
            CHECK_THROWS_AS(augment(A, B), std::invalid_argument);
        }

        SUBCASE("happy path") {
            Matrix I = la::identity(3);
            // clang-format off
            Matrix expected(3, 6, {
                1, -1,  2, 1, 0, 0,
                1,  2, -1, 0, 1, 0,
                0,  2, -2, 0, 0, 1
            });
            // clang-format on
            CHECK_EQ(augment(A, I), expected);
        }
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

TEST_CASE("are_linearly_independent") {
    using la::are_linearly_independent;
    using la::Matrix;

    SUBCASE("zero matrices") {
        CHECK(are_linearly_independent(std::vector<Matrix>{}));
    }

    SUBCASE("different size matrices") {
        Matrix A1(2, 2);
        Matrix A2(2, 3);
        std::vector<Matrix> matrices{A1, A2};
        CHECK_THROWS_AS(are_linearly_independent(matrices),
                        std::invalid_argument);
    }

    SUBCASE("single non-zero matrix is linearly independent") {
        Matrix A(2, 2, {1, 2, 3, 4});
        std::vector<Matrix> matrices{A};
        CHECK(are_linearly_independent(matrices));
    }

    SUBCASE("single zero matrix is not linearly independent") {
        Matrix A(2, 2, {0, 0, 0, 0});
        std::vector<Matrix> matrices{A};
        CHECK(!are_linearly_independent(matrices));
    }

    SUBCASE("true") {
        // Poole example 3.18 p. 155
        Matrix A1(2, 2, {0, 1, -1, 0});
        Matrix A2(2, 2, {1, 0, 0, 1});
        Matrix A3(2, 2, {1, 1, 1, 1});
        std::vector<Matrix> matrices{A1, A2, A3};
        CHECK(are_linearly_independent(matrices));
    }

    SUBCASE("false") {
        // Poole exercise 3.2 14 p. 159
        Matrix A1(2, 2, {1, 1, 1, 1});
        Matrix A2(2, 2, {2, 1, -1, 0});
        Matrix A3(2, 2, {1, 2, 4, 3});
        std::vector<Matrix> matrices{A1, A2, A3};
        CHECK(!are_linearly_independent(matrices));
    }
}
