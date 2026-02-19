#include "doctest/doctest.h"
#include "la/matrix.hpp"
#include "la/pivot_info.hpp"
#include "la/row_reduction.hpp"
#include "test_utils.hpp"

TEST_CASE("is_ref returns true for REF that is normalised") {
    using la::Matrix;
    // clang-format off
    Matrix m(3, 3, {
                       1, 4, 1,
                       0, 1, 2,
                       0, 0, 0
                   });
    // clang-format on
    CHECK(is_ref(m));
}

TEST_CASE("is_ref returns true for REF that is not normalised") {
    using la::Matrix;
    // ADL (Argument-Dependent Lookup) at work here:
    // The compiler automatically searches the namespace of Matrix for the
    // function that has Matrix as its argument (is_ref) and finds it without
    // using la::is_ref;
    // clang-format off
    Matrix m(3, 3, {
                       2,  4, 1,
                       0, -1, 2,
                       0,  0, 0
                   });
    // clang-format on
    CHECK(is_ref(m));
}

TEST_CASE("is_ref returns false for zero row not at the bottom") {
    using la::Matrix;
    // clang-format off
    Matrix m(3, 3, {
                       2, 4, 1,
                       0, 0, 0,
                       0, -1, 2
                   }); // zero row at 2
    // clang-format on
    CHECK(!is_ref(m));
}

TEST_CASE("is_ref returns false for not REF") {
    using la::Matrix;
    // clang-format off
    Matrix m(3, 3, {
                       1, 0, 1,
                       0, 0, 3,
                       0, 1, 0
                   });
    // clang-format on
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
    // clang-format off
    Matrix m(4, 5, {
                       1,  2, -4, -4, 5,
                       2,  4,  0,  0, 2,
                       2,  3,  2,  1, 5,
                       -1, 1,  3,  6, 5
                   });
    // clang-format on
    // Since REF is not unique, we assert these properties:
    // 1. It is in REF and normalised
    // 2. It has same dimensions as original matrix

    Matrix r = ref(m);
    CHECK(is_ref(r));
    CHECK(r.has_same_dimensions(m));
}

TEST_CASE("ref handles correctly a zero row at the beginning") {
    using la::Matrix;
    // clang-format off
    Matrix m(4, 5, {
                       0, 0, 0, 0, 0,
                       2, 4, 0, 0, 2,
                       2, 3, 2, 1, 5,
                       -1, 1, 3, 6, 5
                   });
    // clang-format on
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

    SUBCASE("rref() first row first col is near zero") {
        // clang-format off
        Matrix m(3, 3, {
                           1e-14, 1, 1,
                           1,     1, 1,
                           1,     1, 1
                       });
        Matrix expected(3, 3, {
                                  1, 0, 0,
                                  0, 1, 1,
                                  0, 0, 0
                              });
        // clang-format on
        Matrix actual = rref(m);
        CHECK_NEAR(expected, actual);
    }
}

TEST_CASE("rref eliminates using the last pivot row") {
    using la::is_zero_pivot;
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
        CHECK(is_zero_pivot(std::fabs(R(0, 2))));
        CHECK(is_zero_pivot(std::fabs(R(1, 2))));
    }
}
