#include "la/matrix.hpp"
#include "doctest/doctest.h"
#include "la/utils.hpp"
#include "la/vector.hpp"
#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace la {

Matrix::Matrix(std::size_t rows, std::size_t cols,
               std::initializer_list<double> data)
    : rows_(rows), cols_(cols) {
    if (static_cast<size_t>(rows * cols) != data.size()) {
        throw std::out_of_range{
            "Matrix dimensions did not match with elements in data"};
    }
    data_ = data; // Initialize data only after the check
}

Matrix::Matrix(std::size_t rows, std::size_t cols,
               const std::vector<double> &data)
    : rows_(rows), cols_(cols) {
    // std::cout << "rows=" << rows << " cols=" << cols << " data.size()=" <<
    // data.size()
    //<< " rows*cols=" << rows * cols << "\n";

    if (static_cast<size_t>(rows * cols) != data.size()) {
        throw std::out_of_range{
            "Matrix dimensions did not match with elements in data"};
    }
    data_ = data; // Initialize data only after the check
}

Matrix::Matrix(std::size_t rows, std::size_t cols, const Vector &v) {
    if (v.size() != rows * cols) {
        throw std::out_of_range(
            "Matrix dimensions and vector size do not match");
    }
    rows_ = rows;
    cols_ = cols;
    data_.assign(v.begin(), v.end());
}

Matrix Matrix::operator+(const Matrix &m) const {
    if (!has_same_dimensions(m)) {
        throw std::invalid_argument(
            "Matrix dimensions must match for addition");
    }

    Matrix result(rows_, cols_);

    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < cols_; j++) {
            result(i, j) = (*this)(i, j) + m(i, j);
        }
    }

    return result;
}

Matrix Matrix::operator-(const Matrix &m) const {
    if (!has_same_dimensions(m)) {
        throw std::invalid_argument(
            "Matrix dimensions must match for addition");
    }

    Matrix result(rows_, cols_);

    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < cols_; j++) {
            result(i, j) = (*this)(i, j) - m(i, j);
        }
    }

    return result;
}

Matrix Matrix::operator*(double c) const {
    Matrix result(rows_, cols_);

    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < cols_; j++) {
            result(i, j) = (*this)(i, j) * c;
        }
    }

    return result;
}

Matrix Matrix::operator*(const Matrix &m) const {
    if (cols_ != m.rows()) {
        throw std::invalid_argument(
            "Left matrix columns must match right matrix rows");
    }

    Matrix result(rows_, m.cols());

    for (size_t i = 0; i < rows_; i++) {
        Vector r = row(i);

        for (size_t j = 0; j < m.cols(); j++) {
            Vector col = m.column(j);
            result(i, j) = r.dot_product(col);
        }
    }

    return result;
}

Vector Matrix::operator*(const Vector &v) const {
    Matrix col_matrix(v.size(), 1, v);
    Matrix result = (*this) * col_matrix;
    return result.column(0);
}

Vector Matrix::row(int i) const {
    if (i < 0 || static_cast<size_t>(i) >= rows_) {
        throw std::out_of_range{"Row index does not match matrix dimensions"};
    }

    Vector v(cols_);
    for (size_t j = 0; j < cols_; j++) {
        v[j] = (*this)(i, j);
    }
    return v;
}

Vector Matrix::column(int i) const {
    if (i < 0 || static_cast<size_t>(i) >= cols_) {
        throw std::out_of_range{
            "Column index does not match matrix dimensions"};
    }

    Vector v(rows_);
    for (size_t j = 0; j < rows_; j++) {
        v[j] = (*this)(j, i);
    }
    return v;
}

void Matrix::set_row(size_t row, const Vector &v) {
    assert(v.size() == cols_);
    assert(row < rows_);
    std::copy_n(v.data(), cols_, pointer_to_row_unchecked(row));
}

Matrix Matrix::row_range(std::size_t lower, std::size_t upper) const {
    assert(lower <= upper);
    assert(upper <= rows_);

    Matrix new_matrix = Matrix(upper - lower, cols_);
    for (size_t i = lower; i < upper; i++) {
        new_matrix.set_row(i - lower, row(i));
    }

    return new_matrix;
}

int Matrix::get_leftmost_non_zero_column_index(int leading_row) const {
    for (size_t i = 0; i < cols_; i++) {
        Vector column = (*this).column(i).subvector(leading_row);
        if (!column.is_zero())
            return i;
    }
    return -1;
}

void Matrix::exchange_rows(std::size_t idx_a, std::size_t idx_b) {
    Vector vector_a = row(idx_a);
    Vector vector_b = row(idx_b);
    set_row(idx_a, vector_b);
    set_row(idx_b, vector_a);
}

TEST_CASE("m x n Zero Matrix()") {
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
    CHECK_THROWS_AS(Matrix(-1, 1), std::invalid_argument);
    CHECK_THROWS_AS(Matrix(1, -1), std::invalid_argument);
}

TEST_CASE("Construct matrix of size 0 does not throw") {
    Matrix m(0, 0);
    CHECK(m.rows() == 0);
    CHECK(m.cols() == 0);
}

TEST_CASE("Construct Matrix with initializer, happy case") {
    Matrix m(2, 2, {0, 1, 2, 3});
    CHECK(m.rows() == 2);
    CHECK(m.cols() == 2);
    CHECK(m(0, 0) == 0);
    CHECK(m(0, 1) == 1);
    CHECK(m(1, 0) == 2);
    CHECK(m(1, 1) == 3);
}

TEST_CASE("Construct Matrix with initializer, wrong size") {
    CHECK_THROWS_AS(Matrix m(2, 2, {0, 1, 2}), std::out_of_range);
    CHECK_THROWS_AS(Matrix m(2, 2, {0, 1, 2, 3, 4}), std::out_of_range);
}

TEST_CASE("Matrix checked element access") {
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
    Matrix a(2, 2, {1, 2, 3, 4});
    Matrix b(2, 2, {1, 2, 3, 4});
    CHECK_EQ(a, b);
}

TEST_CASE(
    "Matrices with different dimensions and same elements are not equal") {
    Matrix a(2, 2, {1, 2, 3, 4});
    Matrix b(1, 4, {1, 2, 3, 4});
    CHECK(!(a == b));
}

TEST_CASE(
    "Matrices with same dimensions and different elements are not equal") {
    Matrix a(2, 2, {1, 2, 3, 4});
    Matrix b(2, 2, {1, 2, 3, 100});
    CHECK(!(a == b));
}

TEST_CASE("Get Matrix row, happy case") {
    Matrix m(3, 3, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    Vector v = m.row(1);
    Vector expected{4, 5, 6};
    CHECK_EQ(v, expected);
}

TEST_CASE("Get Matrix row, negative index") {
    Matrix m(3, 3, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    CHECK_THROWS_AS(m.row(-1), std::out_of_range);
}

TEST_CASE("Get Matrix row, index larger than last row index") {
    Matrix m(3, 3, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    CHECK_THROWS_AS(m.row(3), std::out_of_range);
}

TEST_CASE("Get Matrix column, happy case") {
    Matrix m(3, 3, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    Vector v = m.column(1);
    Vector expected{2, 5, 8};
    CHECK_EQ(v, expected);
}

TEST_CASE("Get Matrix column, negative index") {
    Matrix m(3, 3, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    CHECK_THROWS_AS(m.column(-1), std::out_of_range);
}

TEST_CASE("Get Matrix column, index larger than last row index") {
    Matrix m(3, 3, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    CHECK_THROWS_AS(m.column(3), std::out_of_range);
}

TEST_CASE("Same dimensions") {
    Matrix a(2, 2);
    Matrix b(2, 2);
    CHECK(a.has_same_dimensions(b));
    CHECK(b.has_same_dimensions(a));
}

TEST_CASE("Has same dimensions, different rows") {
    Matrix a(2, 2);
    Matrix b(3, 2);
    CHECK(!(a.has_same_dimensions(b)));
    CHECK(!(b.has_same_dimensions(a)));
}

TEST_CASE("Has same dimensions, different columns") {
    Matrix a(2, 2);
    Matrix b(2, 3);
    CHECK(!(a.has_same_dimensions(b)));
    CHECK(!(b.has_same_dimensions(a)));
}

TEST_CASE("Operator +, happy case") {
    Matrix a(2, 2, {1, 2, 3, 4});
    Matrix b(2, 2, {8, 6, 5, 4});
    Matrix expected(2, 2, {9, 8, 8, 8});
    CHECK_EQ(a + b, expected);
}

TEST_CASE("Operator +, different dimensions") {
    Matrix a(2, 2);
    Matrix b(2, 3);
    CHECK_THROWS_AS(a + b, std::invalid_argument);
}

TEST_CASE("Operator -, happy case") {
    Matrix a(2, 2, {1, 2, 3, 4});
    Matrix b(2, 2, {8, 6, 5, 4});
    Matrix expected(2, 2, {-7, -4, -2, 0});
    CHECK_EQ(a - b, expected);
}

TEST_CASE("Operator -, different dimensions") {
    Matrix a(2, 2);
    Matrix b(2, 3);
    CHECK_THROWS_AS(a + b, std::invalid_argument);
}

TEST_CASE("Matrix * scalar") {
    Matrix m(2, 2, {1, 2, 3, 4});
    Matrix expected(2, 2, {2, 4, 6, 8});
    CHECK_EQ(m * 2, expected);
}

TEST_CASE("Matrix * matrix happy case") {
    Matrix a(2, 3, {1, 3, -1, -2, -1, 1});
    Matrix b(3, 4, {-4, 0, 3, -1, 5, -2, -1, 1, -1, 2, 0, 6});
    Matrix expected(2, 4, {12, -8, 0, -4, 2, 4, -5, 7});
    CHECK_EQ(a * b, expected);
}

TEST_CASE("Matrix * matrix with wrong dimensions") {
    Matrix a(3, 2);
    Matrix b(4, 3); // Should be 2 to be able to multiply
    CHECK_THROWS_AS(a * b, std::invalid_argument);
}

TEST_CASE("Matrix *  vector happy case") {
    Matrix m(2, 3, {1, 2, 3, 4, 5, 6});
    Vector v{7, 8, 9};
    Vector expected{50, 122};
    CHECK_EQ(m * v, expected);
}

TEST_CASE("Matrix *  vector wrong dimensions") {
    Matrix m(2, 2, {1, 2, 3, 4}); // need 3 columns to multiply
    Vector v{7, 8, 9};
    CHECK_THROWS_AS(m * v, std::invalid_argument);
}

TEST_CASE("exchange_rows") { Matrix m(2, 2, {1, 2, 3, 4}); }
} // namespace la
