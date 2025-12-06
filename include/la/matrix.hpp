#ifndef LA_MATRIX_HPP
#define LA_MATRIX_HPP

#include "la/vector.hpp"
#include <cstddef> // size_t
#include <initializer_list>
#include <iostream>
#include <vector>

namespace la {
/**
 * A class for representing an m x n matrix.
 */
class Matrix {
    using size_type = std::size_t;

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
    Matrix(std::size_t rows, std::size_t cols,
           std::initializer_list<double> data);

    /** @return Matrix with size rows x cols from vector data */
    Matrix(std::size_t rows, std::size_t cols,
           const std::vector<double> &data);

    Matrix(std::size_t rows, std::size_t cols, const Vector &v);

    /** @return the element at i,j without range check */
    double operator()(std::size_t i, std::size_t j) const noexcept {
        return data_[i * cols_ + j];
    }

    /** @return the element at i,j, writeable and without range check */
    double &operator()(std::size_t i, std::size_t j) {
        return data_[i * cols_ + j];
    }

    /**
     * Output stream operator for Matrix.
     * Prints the matrix in row-major order, one row per line.
     */
    friend std::ostream &operator<<(std::ostream &os, const Matrix &m) {
        for (size_t i = 0; i < m.rows(); ++i) {
            os << "[ ";
            for (size_t j = 0; j < m.cols(); ++j) {
                os << m(i, j);
                if (j + 1 < m.cols())
                    os << ", ";
            }
            os << " ]\n";
        }
        return os;
    }

    /** @return the element at i,j with range check */
    double at(std::ptrdiff_t i, std::ptrdiff_t j) const {
        return data_[checked_index(i, j)];
    }

    /** @return the element at i,j, writeable and with range check */
    double &at(std::ptrdiff_t i, std::ptrdiff_t j) {
        return data_[checked_index(i, j)];
    }

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
     * @return true if the matrices have same number of rows and columns, false
     * otherwise.
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

    /** @brief swap rows a and b */
    void exchange_rows(std::size_t a, std::size_t b);

    /**
     * @brief Create a new matrix from this, with rows [lower, upper)
     *
     * @param lower the first row to include
     * @param upper the first row not to include
     * @return a new matrix
     *
     * @throws std::out_of_range if upper >= lower or upper >= rows
     */
    Matrix row_range(size_t lower, size_t upper) const;

    /**
     * @brief Create a new matrix from this, with columns [lower, upper)
     *
     * @param lower the first column to include
     * @param upper the first column not to include
     * @return a new matrix
     *
     * @throws std::out_of_range if upper >= lower or upper >= rows
     */
    Matrix col_range(size_t lower, size_t upper) const;

    /**
     * @brief set Vector v as the row
     * @param i the row index to set
     * @param v the content of the row as Vector
     */
    void set_row(size_t i, const Vector &v);

    /**
     * @brief set Vector v as the column
     * @param i the row index to set
     * @param v the content of the column as Vector
     */
    void set_col(size_t i, const Vector &v);
  private:
    double *pointer_to_row_unchecked(std::size_t r) noexcept {
        return data_.data() + r * cols_;
    }

    const double *pointer_to_row_unchecked(std::size_t r) const noexcept {
        return data_.data() + r * cols_;
    }

    int get_leftmost_non_zero_column_index(int row_start_index) const;

    size_type checked_index(std::ptrdiff_t i, std::ptrdiff_t j) const {
        if (i < 0 || j < 0) {
            throw std::out_of_range("negative index");
        }
        const size_type ui = static_cast<size_type>(i);
        const size_type uj = static_cast<size_type>(j);
        if (ui >= rows_) {
            throw std::out_of_range("row index out of range");
        }
        if (uj >= cols_) {
            throw std::out_of_range("column index out of range");
        }
        // safe now: ui < rows_ and uj < cols_
        return ui * cols_ + uj;
    }

    size_t rows_;
    size_t cols_;
    std::vector<double> data_;
};
} // namespace la

#endif
