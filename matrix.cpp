#include "doctest.h"
#include "utils.h"
#include "vector.h"
#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <vector>

/**
 * A class for representing an m x n matrix.
 */
class Matrix {
  public:
    /** @return Matrix with size m x n, or rows x cols, initialised to value */
    Matrix(std::size_t rows, std::size_t cols, double value = 0.0)
        : rows_(rows), cols_(cols), data_(rows * cols, value) {}

    /**
     * @return Matrix with size m x n, or rows x cols, initialised to value
     * @throws std::invalid_argument if rows or cols is negative
     */
    Matrix(int rows, int cols, double value = 0.0)
        : Matrix(utils::check_nonnegative(rows, "row count"),
                 utils::check_nonnegative(cols, "column count"), value) {}

    /** @return Matrix with size rows x cols from initializer list data */
    Matrix(std::size_t rows, std::size_t cols, std::initializer_list<double> data);

    /** @return Matrix with size rows x cols from vector data */
    Matrix(std::size_t rows, std::size_t cols, const std::vector<double> &data);

    Matrix(std::size_t rows, std::size_t cols, const Vector &v);

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
     * Matrix matrix multiplication.
     *
     * @param m the other matrix.
     * @return the result of the multiplication.
     * @throws std::invalid_argument if the number of columns in this
     * matrix does not match the number of rows in m.
     */
    Matrix operator*(const Matrix &m) const;

    /**
     * Matrix vector multiplication.
     *
     * @param v the vector to multiply with
     * @return the result of the multiplication
     * @throws std::invalid_argument if the vector length does not
     * match the number of rows in the matrix.
     */
    Vector operator*(const Vector &v) const;

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

    /**
     * @brief determine whether matrix is in row-echelon form.
     *
     * @return true if it is, false if not.
     */
    bool is_ref() const;

    /**
     * @brief determine whether matrix is in reduced row-echelon form.
     *
     * @return true if it is, false if not.
     */
    bool is_rref() const;

    /**
     * @brief return a row echelon form of this matrix
     *
     * @return a REF version of this matrix
     */
    Matrix ref() const;

  private:
    double *pointer_to_row_unchecked(std::size_t r) noexcept { return data_.data() + r * cols_; }

    const double *pointer_to_row_unchecked(std::size_t r) const noexcept {
        return data_.data() + r * cols_;
    }

    // Create a new matrix from this, with rows [lower, upper)
    Matrix row_range(size_t lower, size_t upper) const;
    void set_row(size_t i, const Vector &v);
    int get_leftmost_non_zero_column_index() const;
    void exchange_rows(std::size_t a, std::size_t b);
    size_t rows_;
    size_t cols_;
    std::vector<double> data_;
};

Matrix::Matrix(std::size_t rows, std::size_t cols, std::initializer_list<double> data)
    : rows_(rows), cols_(cols) {
    if (static_cast<size_t>(rows * cols) != data.size()) {
        throw std::out_of_range{"Matrix dimensions did not match with elements in data"};
    }
    data_ = data; // Initialize data only after the check
}

Matrix::Matrix(std::size_t rows, std::size_t cols, const std::vector<double> &data)
    : rows_(rows), cols_(cols) {
    // std::cout << "rows=" << rows << " cols=" << cols << " data.size()=" << data.size()
    //<< " rows*cols=" << rows * cols << "\n";

    if (static_cast<size_t>(rows * cols) != data.size()) {
        throw std::out_of_range{"Matrix dimensions did not match with elements in data"};
    }
    data_ = data; // Initialize data only after the check
}

Matrix::Matrix(std::size_t rows, std::size_t cols, const Vector &v) {
    if (v.size() != rows * cols) {
        throw std::out_of_range("Matrix dimensions and vector size do not match");
    }
    rows_ = rows;
    cols_ = cols;
    data_.assign(v.begin(), v.end());
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

Matrix Matrix::operator*(const Matrix &m) const {
    if (cols_ != m.rows()) {
        throw std::invalid_argument("Left matrix columns must match right matrix rows");
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
        throw std::out_of_range{"Column index does not match matrix dimensions"};
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

bool Matrix::is_ref() const {
    /*
     * A matrix is in row echelon form if it statisfies the following
     * properties:
     * 1. Any rows consisting entirely of zeros are at the bottom.
     * 2. In each nonzero row, the first nonzero entry (leading entry) is in a
     *    column to the left of any leading entries below it.
     */

    // Assert zero rows are at the bottom:
    // 1. find first zero row starting from top and record its presence
    // 2. if any non-zero rows are found below it, return false
    bool found_zero = false;
    for (size_t i = 0; i < rows_; i++) {
        Vector v = row(i);
        if (!found_zero) {
            found_zero = v.is_zero();
        } else { // a zero row has been found before this row
            // from now on, it is allowed to have only zero rows
            if (!v.is_zero()) {
                return false;
            }
        }
    }
    // All zero rows are at the bottom.

    // Assert leading entry index grows when going down non-zero vectors
    int prev_leading_entry_column = -1; // valid columns indexed from 0 to m-1
    for (size_t i = 0; i < rows_; i++) {
        Vector v = row(i);
        // When we find first zero vector, there will be no more leading
        // enries to check and matrix is in row-echelon form.
        if (v.is_zero()) {
            return true;
        }

        // Assert this leading entry column index is greater than previous.
        int cur_leading_entry_column = v.first_non_zero_column();
        if (cur_leading_entry_column <= prev_leading_entry_column) {
            return false;
        }
        prev_leading_entry_column = cur_leading_entry_column;
    }

    // Both requirements hold, the matrix is in row echelon form.
    return true;
}

bool Matrix::is_rref() const {
    // 1. Is in row echelon form
    if (!is_ref()) {
        std::cout << "Not REF\n";
        return false;
    }

    for (size_t i = 0; i < rows_; i++) {
        // 2. The leading entry in each nonzero row is a leading 1
        Vector v = row(i);
        if (v.is_zero()) {
            continue; // no leading entry in zero row
        }
        auto leading_element = v.leading_element();
        if (leading_element != 1) {
            std::cout << "Row " << i << " leading element is " << leading_element << "\n";
            return false;
        }

        // 3. Each column containing a leading 1 is standard basis vector
        int leading_entry_column = v.first_non_zero_column();
        Vector col = column(leading_entry_column);
        if (!col.is_standard_basis()) {
            std::cout << "Column " << leading_entry_column << " is not standard basis\n";
            return false;
        }
    }

    return true;
}

int Matrix::get_leftmost_non_zero_column_index() const {
    for (size_t i = 0; i < cols_; i++) {
        Vector column = (*this).column(i);
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

Matrix Matrix::ref() const {
    Matrix r = *this; // copy this matrix

    if (r.is_ref()) {
        return r; // already in REF
    }

    // Guidelines from Poole, Linear Algebra: A Modern Introduction, 2nd ed, pp 72-73
    //
    // For each row as top row, starting with the top row of the whole matrix:
    //   1. Locate the leftmost non-zero column of the rows below (and including) the top row
    //   2. Create a leading entry in the top row by interchanging it with the top row
    //   3. Create a leading 1 by scaling the row
    //   4. Use the leading 1 to create zeros below it
    for (size_t top_row_idx = 0; top_row_idx < r.rows_; top_row_idx++) {
        // 0. Take the remaining matrix: from top_row_idx to end:
        Matrix remaining = r.row_range(top_row_idx, r.rows_);

        // 1. Locate the leftmost non-zero column of the rows below (and including) the top row
        int leftmost_non_zero_column_index = remaining.get_leftmost_non_zero_column_index();
        if (leftmost_non_zero_column_index != -1) // no non-zero columns left, return
            break;                                // or break out of the loop?

        // 2. Create a leading entry in the top row by interchanging it with the top row
        Vector pivot_col = remaining.column(leftmost_non_zero_column_index);
        int row_idx = pivot_col.first_non_zero_column();
        assert(row_idx != -1); // Should not be possible, but assert
        remaining.exchange_rows(top_row_idx, row_idx);

        // 3. Create a leading 1 by scaling the row
        Vector top_row = remaining.row(0);
        double leading_element_value = top_row.leading_element();
        Vector scaled_row = top_row * (1 / leading_element_value);
        remaining.set_row(0, scaled_row);
    }

    return r;
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

TEST_CASE("is_ref returns true for REF") {
    Matrix m(3, 3, {2, 4, 1, 0, -1, 2, 0, 0, 0});
    CHECK(m.is_ref());
}

TEST_CASE("is_ref returns false for zero row not at the bottom") {
    Matrix m(3, 3, {2, 4, 1, 0, 0, 0, 0, -1, 2}); // zero row at 2
    CHECK(!m.is_ref());
}

TEST_CASE("is_ref returns false for not REF") {
    Matrix m(3, 3, {1, 0, 1, 0, 0, 3, 0, 1, 0});
    CHECK(!m.is_ref());
}

TEST_CASE("is_rref returns true for RREF") {
    Matrix m(5, 7, {1, 2,  0, 0, -3, 1, 0, 0, 0, 1, 0, 4, -1, 0, 0, 0, 0, 1,
                    3, -2, 0, 0, 0,  0, 0, 0, 0, 1, 0, 0, 0,  0, 0, 0, 0});
    CHECK(m.is_rref());
}

TEST_CASE("is_rref returns false for column containing leading 1 not a standard basis vector") {
    Matrix m(3, 3, {1, 0, 1, 0, 1, 0, 0, 0, 1});
    CHECK(!m.is_rref());
}

TEST_CASE("is_rref returns false for leading entry not 1") {
    Matrix m(3, 3, {2, 0, 0, 0, 1, 0, 0, 0, 1});
    CHECK(!m.is_rref());
}

TEST_CASE("is_rref returns false for not REF") {
    Matrix m(3, 3, {1, 0, 1, 0, 0, 3, 0, 1, 0});
    CHECK(!m.is_rref());
}

TEST_CASE("is_rref returns true for zero matrix") {
    Matrix m(3, 3, 0.0);
    CHECK(m.is_rref());
}

TEST_CASE("is_rref returns true for 1x1 matrix with 0") {
    Matrix m(1, 1, 0.0);
    CHECK(m.is_rref());
}

TEST_CASE("is_rref returns true for 1x1 matrix with 1") {
    Matrix m(1, 1, 1.0);
    CHECK(m.is_rref());
}

TEST_CASE("is_rref returns false for 1x1 matrix with value other than 0 or 1") {
    Matrix m(1, 1, 2.0);
    CHECK(!m.is_rref());
}

TEST_CASE("is_rref returns true for empty matrix") {
    Matrix m(0, 0);
    CHECK(m.is_rref());
}

TEST_CASE("exchange_rows") { Matrix m(2, 2, {1, 2, 3, 4}); }

// TEST_CASE("ref returns a matrix that is in REF") {
//     Matrix m(4, 5, {
//         1, 2, -4, -4, 5,
//         2, 4, 0, 0, 2,
//         2, 3, 2, 1, 5,
//         -1, 1, 3, 6, 5
//     });
//     // Since REF is not unique, we assert these properties:
//     // 1. It is in REF
//     // 2. It has same dimensions as original matrix

//     Matrix r = m.ref();
//     CHECK(r.is_ref());
//     CHECK(r.has_same_dimensions(m));
// }
