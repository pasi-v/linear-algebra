#include "doctest.h"
#include <stdexcept>
#include <vector>

/**
 * A class for representing an m x n matrix.
 */
class Matrix {
  public:
    /** @return Matrix with size m x n, or rows x cols, initialised to value */
    Matrix(size_t rows, size_t cols, double value = 0.0);

    /** @return the element at i,j without range check */
    double operator()(size_t i, size_t j) const { return data_.at(i * cols_ + j); }

    /** @return rows */
    size_t rows() const { return rows_; }

    /** @return columns */
    size_t cols() const { return cols_; }

  private:
    size_t rows_;
    size_t cols_;
    std::vector<double> data_;
};

Matrix::Matrix(size_t rows, size_t cols, double value)
    // construct the matrix with rows x cols default-initialised elements
    : rows_(rows), cols_(cols), data_(rows * cols, value) {
    if (rows < 0 || cols < 0)
        throw std::length_error{"Matrix size can't be negative"};
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
    CHECK_THROWS_AS(Matrix(-1, 1), std::length_error);
    CHECK_THROWS_AS(Matrix(1, -1), std::length_error);
}

TEST_CASE("Construct matrix of size 0 does not throw") {
    Matrix m(0, 0);
    CHECK(m.rows() == 0);
    CHECK(m.cols() == 0);
}
