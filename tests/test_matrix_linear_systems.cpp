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
    using la::is_in_span;
    using la::Vector;
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
