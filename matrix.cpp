#include "doctest.h"
#include <initializer_list>
#include <stdexcept>
#include <vector>
#include "vector.h"

/**
 * A class for representing an m x n matrix.
 */
class Matrix {
  public:
    /** @return Matrix with size m x n, or rows x cols, initialised to value */
    Matrix(size_t rows, size_t cols, double value = 0.0);

    /** @return Matrix with size rows x cols from initializer list data */
    Matrix(size_t rows, size_t cols, std::initializer_list<double> data);

    /** @return the element at i,j without range check */
    double operator()(size_t i, size_t j) const { return data_.at(i * cols_ + j); }

    /** @return true if the matrices have same dimensions and elements */
    friend bool operator==(const Matrix &a, const Matrix &b) {
        return a.rows_ == b.rows_ && a.cols_ == b.cols_ && a.data_ == b.data_;
    }
    
    /** @return row */
    Vector row(size_t i) const {
		Vector v(cols_);
		for (size_t j = 0; j < cols_; j++) {
			v[j] = (*this)(i, j);
		}
		return v;
	}

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

Matrix::Matrix(size_t rows, size_t cols, std::initializer_list<double> data)
    : rows_(rows), cols_(cols) {
    if (rows < 0 || cols < 0 || rows * cols != data.size()) {
        throw std::length_error{"Matrix dimensions did not match with elements in data"};
    }
    data_ = data; // Initialize data only after the check
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
    CHECK_THROWS_AS(Matrix m(2, 2, {0, 1, 2}), std::length_error);
    CHECK_THROWS_AS(Matrix m(2, 2, {0, 1, 2, 3, 4}), std::length_error);
}

TEST_CASE("Matrices with same dimensions and elements are equal") {
	Matrix a(2, 2, {1, 2, 3, 4});
	Matrix b(2, 2, {1, 2, 3, 4});
	CHECK_EQ(a, b);
}

TEST_CASE("Matrices with different dimensions and same elements are not equal") {
	Matrix a(2, 2, {1, 2, 3, 4});
	Matrix b(1, 4, {1, 2, 3, 4});
	CHECK(!(a == b));
}

TEST_CASE("Matrices with same dimensions and different elements are not equal") {
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
