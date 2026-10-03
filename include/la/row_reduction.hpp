#ifndef LA_ROW_REDUCTION_HPP
#define LA_ROW_REDUCTION_HPP

#include "la/matrix.hpp"

#include "pivot_info.hpp"

namespace la {
struct EchelonResult {
    Matrix R;
    PivotInfo pivots;
};

/**
 * @brief determine whether matrix is in row-echelon form.
 *
 * @return true if it is, false if not.
 */
bool is_ref(const Matrix &A);

/**
 * @brief determine whether matrix is in reduced row-echelon form.
 *
 * @return true if it is, false if not.
 */
bool is_rref(const Matrix &A);

/**
 * @brief return a row echelon form of matrix
 * @param A the matrix
 * @return a REF version of this matrix and its pivots
 */
EchelonResult ref(const Matrix &A);

/**
 * @brief return a reduced row echelon form of matrix
 * @param A the matrix
 * @return a RREF version of A and its pivots
 */
EchelonResult rref(const Matrix &A);

/**
 * @return the number on nonzero rows in row echelon form
 */
std::size_t rank(const Matrix &A);
} // namespace la

#endif // LA_ROW_REDUCTION_HPP
