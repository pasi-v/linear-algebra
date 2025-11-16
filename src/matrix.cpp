#include "la/matrix.hpp"
#include "la/vector.hpp"
#include "utils/utils.hpp"
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
    if (lower > upper) {
        throw std::range_error("upper must be greater than lower");
    }
    if (upper > rows()) {
        throw std::range_error("upper must be less than or equal to rows");
    }

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
} // namespace la
