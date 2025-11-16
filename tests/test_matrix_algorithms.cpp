#include "doctest/doctest.h"
#include "la/matrix.hpp"
#include "la/matrix_algorithms.hpp"
#include <cmath>

TEST_CASE("is_ref returns true for REF") {
    using la::Matrix;
    // ADL (Argument-Dependent Lookup) at work here:
    // The compiler automatically searches the namespace of Matrix for the
    // function that has Matrix as its argument (is_ref) and finds it without
    // using la::is_ref;
    Matrix m(3, 3, {2, 4, 1, 0, -1, 2, 0, 0, 0});
    CHECK(is_ref(m));
}

TEST_CASE("is_ref returns false for zero row not at the bottom") {
    using la::Matrix;
    Matrix m(3, 3, {2, 4, 1, 0, 0, 0, 0, -1, 2}); // zero row at 2
    CHECK(!is_ref(m));
}

TEST_CASE("is_ref returns false for not REF") {
    using la::Matrix;
    Matrix m(3, 3, {1, 0, 1, 0, 0, 3, 0, 1, 0});
    CHECK(!is_ref(m));
}

TEST_CASE("is_rref returns true for RREF") {
    using la::Matrix;
    Matrix m(5, 7, {1, 2,  0, 0, -3, 1, 0, 0, 0, 1, 0, 4, -1, 0, 0, 0, 0, 1,
                    3, -2, 0, 0, 0,  0, 0, 0, 0, 1, 0, 0, 0,  0, 0, 0, 0});
    CHECK(is_rref(m));
}

TEST_CASE("is_rref returns false for column containing leading 1 not a "
          "standard basis vector") {
    using la::Matrix;
    Matrix m(3, 3, {1, 0, 1, 0, 1, 0, 0, 0, 1});
    CHECK(!is_rref(m));
}

TEST_CASE("is_rref returns false for leading entry not 1") {
    using la::Matrix;
    Matrix m(3, 3, {2, 0, 0, 0, 1, 0, 0, 0, 1});
    CHECK(!is_rref(m));
}

TEST_CASE("is_rref returns false for not REF") {
    using la::Matrix;
    Matrix m(3, 3, {1, 0, 1, 0, 0, 3, 0, 1, 0});
    CHECK(!is_rref(m));
}

TEST_CASE("is_rref returns true for zero matrix") {
    using la::Matrix;
    Matrix m(3, 3, 0.0);
    CHECK(is_rref(m));
}

TEST_CASE("is_rref returns true for 1x1 matrix with 0") {
    using la::Matrix;
    Matrix m(1, 1, 0.0);
    CHECK(is_rref(m));
}

TEST_CASE("is_rref returns true for 1x1 matrix with 1") {
    using la::Matrix;
    Matrix m(1, 1, 1.0);
    CHECK(is_rref(m));
}

TEST_CASE(
    "is_rref returns false for 1x1 matrix with value other than 0 or 1") {
    using la::Matrix;
    Matrix m(1, 1, 2.0);
    CHECK(!is_rref(m));
}

TEST_CASE("is_rref returns true for empty matrix") {
    using la::Matrix;
    Matrix m(0, 0);
    CHECK(is_rref(m));
}

TEST_CASE("ref returns a matrix that is in REF") {
    using la::Matrix;
    Matrix m(4, 5,
             {1, 2, -4, -4, 5, 2, 4, 0, 0, 2, 2, 3, 2, 1, 5, -1, 1, 3, 6, 5});
    // Since REF is not unique, we assert these properties:
    // 1. It is in REF
    // 2. It has same dimensions as original matrix

    Matrix r = ref(m);
    CHECK(is_ref(r));
    CHECK(r.has_same_dimensions(m));
}

TEST_CASE("ref handles correctly a zero row at the beginning") {
    using la::Matrix;
    Matrix m(4, 5,
             {0, 0, 0, 0, 0, 2, 4, 0, 0, 2, 2, 3, 2, 1, 5, -1, 1, 3, 6, 5});
    // Since REF is not unique, we assert these properties:
    // 1. It is in REF
    // 2. It has same dimensions as original matrix

    Matrix r = ref(m);
    CHECK(is_ref(r));
    CHECK(r.has_same_dimensions(m));
}

TEST_CASE("ref handles correctly a zero row at the end") {
    using la::Matrix;
    Matrix m(4, 5,
             {-1, 1, 3, 6, 5, 2, 4, 0, 0, 2, 2, 3, 2, 1, 5, 0, 0, 0, 0, 0});
    // Since REF is not unique, we assert these properties:
    // 1. It is in REF
    // 2. It has same dimensions as original matrix

    Matrix r = ref(m);
    CHECK(is_ref(r));
    CHECK(r.has_same_dimensions(m));
}

TEST_CASE("ref handles correctly a zero row in the middle") {
    using la::Matrix;
    Matrix m(4, 5,
             {-1, 1, 3, 6, 5, 2, 4, 0, 0, 2, 0, 0, 0, 0, 0, 2, 3, 2, 1, 5});
    // Since REF is not unique, we assert these properties:
    // 1. It is in REF
    // 2. It has same dimensions as original matrix

    Matrix r = ref(m);
    CHECK(is_ref(r));
    CHECK(r.has_same_dimensions(m));
}

TEST_CASE("ref handles correctly a zero matrix") {
    using la::Matrix;
    Matrix m(3, 3, {0, 0, 0, 0, 0, 0, 0, 0, 0});
    Matrix r = ref(m);
    CHECK(is_ref(r));
    CHECK(r.has_same_dimensions(m));
}

TEST_CASE("ref handles correctly a matrix without zero rows") {
    using la::Matrix;
    Matrix m(3, 5, {1, -1, -1, 2, 1, 2, -2, -1, 3, 3, -1, 1, -1, 0, -3});
    Matrix r = ref(m);
    CHECK(is_ref(r));
    CHECK(r.has_same_dimensions(m));
}

TEST_CASE("rank") {
    using la::Matrix;
    SUBCASE("zero matrix") {
        Matrix m(2, 2, {0, 0, 0, 0});
        CHECK_EQ(rank(m), 0);
    }

    SUBCASE("rank less than rows") {
        Matrix m(3, 5, {1, -1, -1, 2, 1, 2, -2, -1, 3, 3, -1, 1, -1, 0, -3});
        CHECK_EQ(rank(m), 2);
    }

    SUBCASE("rank equal to rows") {
        Matrix m(4, 5, {1, 2, -4, -4, 5, 2,  4, 0, 0, 2,
                        2, 3, 2,  1,  5, -1, 1, 3, 6, 5});
        CHECK_EQ(rank(m), 4);
    }
}

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

TEST_CASE("Reduced Row Echelon Form") {
    using la::Matrix;

    SUBCASE("rref() returns equal to itself if already in RREF") {
        // clang-format off
        Matrix m(3, 5, {
            1, -1, 0,  1, 2,
            0,  0, 1, -1, 1,
            0,  0, 0,  0, 0
        });
        // clang-format on
        CHECK_EQ(m, rref(m));
    }

    SUBCASE("rref() happy case") {
        // clang-format off
        Matrix m(3, 5, {
             1, -1, -1, 2,  1,
             2, -2, -1, 3,  3,
            -1,  1, -1, 0, -3
        });
        Matrix expected(3, 5, {
            1, -1, 0,  1, 2,
            0,  0, 1, -1, 1,
            0,  0, 0,  0, 0
        });
        // clang-format on
        CHECK_EQ(expected, rref(m));
    }
}

TEST_CASE("rref eliminates using the last pivot row") {
    using la::Matrix;

    // Construct a matrix whose REF has pivots in all 3 rows.
    // The last pivot is in the last row; its column has a nonzero above.
    // clang-format off
    Matrix A(3, 3, {
        1, 2, 0,
        0, 1, 3,
        0, 0, 1
    });
    // clang-format on

    Matrix R = la::rref(A);

    SUBCASE("all entries above last pivot are zero") {
        // The pivot in column 2 should have zeros above it.
        CHECK(std::fabs(R(0, 2)) <= la::kEps);
        CHECK(std::fabs(R(1, 2)) <= la::kEps);
    }
}

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
