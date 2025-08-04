#include "doctest.h"
#include "vector.h"
#include <initializer_list>
#include <stdexcept>
#include <vector>

/**
 * A class for representing an m x n matrix.
 */
class Matrix {
  public:
    /** @return Matrix with size m x n, or rows x cols, initialised to value */
    Matrix(int rows, int cols, double value = 0.0);

    /** @return Matrix with size rows x cols from initializer list data */
    Matrix(int rows, int cols, std::initializer_list<double> data);

    /** @return the element at i,j without range check */
    double operator()(int i, int j) const { return data_[i * cols_ + j]; }

    /** @return the element at i,j, writeable and without range check */
    double &operator()(int i, int j) { return data_[i * cols_ + j]; }

    /** @return true if the matrices have same dimensions and elements */
    friend bool operator==(const Matrix &a, const Matrix &b) {
        return a.has_same_dimensions(b) && a.data_ == b.data_;
    }

    /**
     * Matrix addition operator.
     *
     * @param m matrix to add to this one.
     * @return the sum of the matrices.
     * @throws std::invalid_argument if the matrix dimensions do not match
     */
    Matrix operator+(const Matrix &m) const;

    /**
     * Matrix subtraction operator.
     *
     * @param m matrix to subtract from this one.
     * @return the difference of the matrices.
     * @throws std::invalid_argument if the matrix dimensions do not match
     */
    Matrix operator-(const Matrix &m) const;

    /**
     * Matrix scalar multiplication.
     *
     * @param c scalar with which to multiply this matrix.
     * @return the matrix multiplied with the scalar.
     */
    Matrix operator*(double c) const;

    /**
     * Determine do the matrices have same dimensions.
     *
     * @param m the other matrix.
     * @return true if the matrices have same number of rows and columns, false otherwise.
     */
    bool has_same_dimensions(const Matrix m) const {
        return rows_ == m.rows() && cols_ == m.cols();
    }

    /**
     * Get row i from the matrix as Vector.
     *
     * @param i the zero-based row index.
     * @return the ith row as Vector.
     * @throws std::out_of_range if i < 0 or i >= number of rows in matrix
     */
    Vector row(int i) const;

    /**
     * Get column i from the matrix as Vector.
     *
     * @param i the zero-based column index.
     * @return the ith column as Vector.
     * @throws std::out_of_range if i < 0 or i >= number of columns in matrix.
     */
    Vector column(int i) const;

    /** @return rows */
    size_t rows() const { return rows_; }

    /** @return columns */
    size_t cols() const { return cols_; }

  private:
    size_t rows_;
    size_t cols_;
    std::vector<double> data_;
};

Matrix::Matrix(int rows, int cols, double value) {
    if (rows < 0 || cols < 0)
        throw std::out_of_range{"Matrix size can't be negative"};
    rows_ = rows;
    cols_ = cols;
    data_ = std::vector<double>(rows * cols, value);
}

Matrix::Matrix(int rows, int cols, std::initializer_list<double> data) : rows_(rows), cols_(cols) {
    if (rows < 0 || cols < 0 || static_cast<size_t>(rows * cols) != data.size()) {
        throw std::out_of_range{"Matrix dimensions did not match with elements in data"};
    }
    data_ = data; // Initialize data only after the check
}

Matrix Matrix::operator+(const Matrix &m) const {
    if (!has_same_dimensions(m)) {
        throw std::invalid_argument("Matrix dimensions must match for addition");
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
        throw std::invalid_argument("Matrix dimensions must match for addition");
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
        throw std::out_of_range{"Column index does not match matrix dimensions"};
    }

    Vector v(rows_);
    for (size_t j = 0; j < rows_; j++) {
        v[j] = (*this)(j, i);
    }
    return v;
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
    CHECK_THROWS_AS(Matrix(-1, 1), std::out_of_range);
    CHECK_THROWS_AS(Matrix(1, -1), std::out_of_range);
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
    Vector expected = Vector::from_values({4, 5, 6});
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
    Vector expected = Vector::from_values({2, 5, 8});
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
