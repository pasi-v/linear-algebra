#include "doctest/doctest.h"
#include "la/matrix.hpp"

TEST_CASE("m x n Zero Matrix()") {
    using la::Matrix;

    size_t m = 3;
    size_t n = 3;

    Matrix mat(m, n);

    CHECK(mat.rows() == m);
    CHECK(mat.cols() == n);
    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < n; j++) {
            CHECK(mat(i, j) == 0.0);
        }
    }
}

TEST_CASE("m x n initialised Matrix()") {
    using la::Matrix;

    size_t m = 2;
    size_t n = 4;
    double value = 1.1;

    Matrix mat(m, n, value);

    CHECK(mat.rows() == m);
    CHECK(mat.cols() == n);
    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < n; j++) {
            CHECK(mat(i, j) == value);
        }
    }
}

TEST_CASE("Construct matrix with negative rows or columns throws") {
    using la::Matrix;

    CHECK_THROWS_AS(Matrix(-1, 1), std::invalid_argument);
    CHECK_THROWS_AS(Matrix(1, -1), std::invalid_argument);
}

TEST_CASE("Construct matrix of size 0 does not throw") {
    using la::Matrix;

    Matrix m(0, 0);
    CHECK(m.rows() == 0);
    CHECK(m.cols() == 0);
}

TEST_CASE("Construct Matrix with initializer, happy case") {
    using la::Matrix;

    Matrix m(2, 2, {0, 1, 2, 3});
    CHECK(m.rows() == 2);
    CHECK(m.cols() == 2);
    CHECK(m(0, 0) == 0);
    CHECK(m(0, 1) == 1);
    CHECK(m(1, 0) == 2);
    CHECK(m(1, 1) == 3);
}

TEST_CASE("Construct Matrix with initializer, wrong size") {
    using la::Matrix;

    CHECK_THROWS_AS(Matrix m(2, 2, {0, 1, 2}), std::out_of_range);
    CHECK_THROWS_AS(Matrix m(2, 2, {0, 1, 2, 3, 4}), std::out_of_range);
}

TEST_CASE("Matrix checked element access") {
    using la::Matrix;

    Matrix m(2, 2, {1, 2, 3, 4});
    CHECK_EQ(1, m.at(0, 0));
    CHECK_EQ(2, m.at(0, 1));
    CHECK_EQ(3, m.at(1, 0));
    CHECK_EQ(4, m.at(1, 1));
    // This would calculate as 3rd element in the data, but is still invalid
    CHECK_THROWS_AS(m.at(0, 2), std::out_of_range);
    CHECK_THROWS_AS(m.at(0, 2), std::out_of_range);
    CHECK_THROWS_AS(m.at(-1, 0), std::out_of_range);
    CHECK_THROWS_AS(m.at(0, -1), std::out_of_range);
}

TEST_CASE("Matrices with same dimensions and elements are equal") {
    using la::Matrix;

    Matrix a(2, 2, {1, 2, 3, 4});
    Matrix b(2, 2, {1, 2, 3, 4});
    CHECK_EQ(a, b);
}

TEST_CASE(
    "Matrices with different dimensions and same elements are not equal") {
    using la::Matrix;

    Matrix a(2, 2, {1, 2, 3, 4});
    Matrix b(1, 4, {1, 2, 3, 4});
    CHECK(!(a == b));
}

TEST_CASE(
    "Matrices with same dimensions and different elements are not equal") {
    using la::Matrix;

    Matrix a(2, 2, {1, 2, 3, 4});
    Matrix b(2, 2, {1, 2, 3, 100});
    CHECK(!(a == b));
}

TEST_CASE("Get Matrix row, happy case") {
    using la::Matrix;
    using la::Vector;

    Matrix m(3, 3, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    Vector v = m.row(1);
    Vector expected{4, 5, 6};
    CHECK_EQ(v, expected);
}

TEST_CASE("Get Matrix row, negative index") {
    using la::Matrix;

    Matrix m(3, 3, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    CHECK_THROWS_AS(m.row(-1), std::out_of_range);
}

TEST_CASE("Get Matrix row, index larger than last row index") {
    using la::Matrix;

    Matrix m(3, 3, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    CHECK_THROWS_AS(m.row(3), std::out_of_range);
}

TEST_CASE("Get Matrix column, happy case") {
    using la::Matrix;
    using la::Vector;

    Matrix m(3, 3, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    Vector v = m.column(1);
    Vector expected{2, 5, 8};
    CHECK_EQ(v, expected);
}

TEST_CASE("Get Matrix column, negative index") {
    using la::Matrix;

    Matrix m(3, 3, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    CHECK_THROWS_AS(m.column(-1), std::out_of_range);
}

TEST_CASE("Get Matrix column, index larger than last row index") {
    using la::Matrix;

    Matrix m(3, 3, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    CHECK_THROWS_AS(m.column(3), std::out_of_range);
}

TEST_CASE("Same dimensions") {
    using la::Matrix;

    Matrix a(2, 2);
    Matrix b(2, 2);
    CHECK(a.has_same_dimensions(b));
    CHECK(b.has_same_dimensions(a));
}

TEST_CASE("Has same dimensions, different rows") {
    using la::Matrix;

    Matrix a(2, 2);
    Matrix b(3, 2);
    CHECK(!(a.has_same_dimensions(b)));
    CHECK(!(b.has_same_dimensions(a)));
}

TEST_CASE("Has same dimensions, different columns") {
    using la::Matrix;

    Matrix a(2, 2);
    Matrix b(2, 3);
    CHECK(!(a.has_same_dimensions(b)));
    CHECK(!(b.has_same_dimensions(a)));
}

TEST_CASE("Operator +, happy case") {
    using la::Matrix;

    Matrix a(2, 2, {1, 2, 3, 4});
    Matrix b(2, 2, {8, 6, 5, 4});
    Matrix expected(2, 2, {9, 8, 8, 8});
    CHECK_EQ(a + b, expected);
}

TEST_CASE("Operator +, different dimensions") {
    using la::Matrix;

    Matrix a(2, 2);
    Matrix b(2, 3);
    CHECK_THROWS_AS(a + b, std::invalid_argument);
}

TEST_CASE("Operator -, happy case") {
    using la::Matrix;

    Matrix a(2, 2, {1, 2, 3, 4});
    Matrix b(2, 2, {8, 6, 5, 4});
    Matrix expected(2, 2, {-7, -4, -2, 0});
    CHECK_EQ(a - b, expected);
}

TEST_CASE("Operator -, different dimensions") {
    using la::Matrix;

    Matrix a(2, 2);
    Matrix b(2, 3);
    CHECK_THROWS_AS(a + b, std::invalid_argument);
}

TEST_CASE("Matrix * scalar") {
    using la::Matrix;

    Matrix m(2, 2, {1, 2, 3, 4});
    Matrix expected(2, 2, {2, 4, 6, 8});
    CHECK_EQ(m * 2, expected);
}

TEST_CASE("Matrix * matrix happy case") {
    using la::Matrix;

    Matrix a(2, 3, {1, 3, -1, -2, -1, 1});
    Matrix b(3, 4, {-4, 0, 3, -1, 5, -2, -1, 1, -1, 2, 0, 6});
    Matrix expected(2, 4, {12, -8, 0, -4, 2, 4, -5, 7});
    CHECK_EQ(a * b, expected);
}

TEST_CASE("Matrix * matrix with wrong dimensions") {
    using la::Matrix;

    Matrix a(3, 2);
    Matrix b(4, 3); // Should be 2 to be able to multiply
    CHECK_THROWS_AS(a * b, std::invalid_argument);
}

TEST_CASE("Matrix *  vector happy case") {
    using la::Matrix;
    using la::Vector;

    Matrix m(2, 3, {1, 2, 3, 4, 5, 6});
    Vector v{7, 8, 9};
    Vector expected{50, 122};
    CHECK_EQ(m * v, expected);
}

TEST_CASE("Matrix *  vector wrong dimensions") {
    using la::Matrix;
    using la::Vector;

    Matrix m(2, 2, {1, 2, 3, 4}); // need 3 columns to multiply
    Vector v{7, 8, 9};
    CHECK_THROWS_AS(m * v, std::invalid_argument);
}

TEST_CASE("exchange_rows") {
    using la::Matrix;

    Matrix m(2, 2, {1, 2, 3, 4});
    // TODO: Call act and asser
}

TEST_SUITE("range methods") {
    using la::Matrix;

    // clang-format off
    Matrix m(3, 4, {
        1,  2,  3,  4,
        5,  6,  7,  8,
        9, 10, 11, 12
    });
    // clang-format on

    TEST_CASE("row_range") {
        SUBCASE("take first rows") {
            Matrix expected(2, 4, {1, 2, 3, 4, 5, 6, 7, 8});
            CHECK_EQ(m.row_range(0, 2), expected);
        }

        SUBCASE("take middle row") {
            Matrix expected(1, 4, {5, 6, 7, 8});
            CHECK_EQ(m.row_range(1, 2), expected);
        }

        SUBCASE("take one last row") {
            Matrix expected(1, 4, {9, 10, 11, 12});
            CHECK_EQ(m.row_range(2, 3), expected);
        }

        SUBCASE("take all rows") { CHECK_EQ(m.row_range(0, 3), m); }

        SUBCASE("take no rows returns zero-row matrix") {
            CHECK_EQ(m.row_range(0, 0), Matrix(0, 4));
        }

        SUBCASE("lower > upper throws") {
            CHECK_THROWS_AS(m.row_range(1, 0), std::range_error);
        }

        SUBCASE("upper > total rows throws") {
            CHECK_THROWS_AS(m.row_range(0, 5), std::range_error);
        }
    }

    TEST_CASE("column range") {
        SUBCASE("take first two columns") {
            Matrix expected(3, 2, {1, 2, 5, 6, 9, 10});
            CHECK_EQ(m.col_range(0, 2), expected);
        }

        SUBCASE("take first column") {
            Matrix expected(3, 1, {1, 5, 9});
            CHECK_EQ(m.col_range(0, 1), expected);
        }

        SUBCASE("take one middle column") {
            Matrix expected(3, 1, {2, 6, 10});
            CHECK_EQ(m.col_range(1, 2), expected);
        }

        SUBCASE("take last column") {
            Matrix expected(3, 1, {4, 8, 12});
            CHECK_EQ(m.col_range(3, 4), expected);
        }

        SUBCASE("take two last columns") {
            Matrix expected(3, 2, {3, 4, 7, 8, 11, 12});
            CHECK_EQ(m.col_range(2, 4), expected);
        }

        SUBCASE("lower > upper throws") {
            CHECK_THROWS_AS(m.col_range(1, 0), std::range_error);
        }

        SUBCASE("upper > total rows throws") {
            CHECK_THROWS_AS(m.col_range(0, 5), std::range_error);
        }
    }
}