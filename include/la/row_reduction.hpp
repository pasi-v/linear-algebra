#ifndef LA_ROW_REDUCTION_HPP
#define LA_ROW_REDUCTION_HPP

#include "la/matrix.hpp"

namespace la {
/**
 * @brief determine whether matrix is in row-echelon form with normalised
 * leading entries (all pivots = 1)
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
 * @brief return a row echelon form of this matrix with normalised leading
 * entries (all pivots = 1)
 *
 * @return a REF version of this matrix
 */
Matrix ref(const Matrix &A);

/**
 * @brief return a reduced row echelon form of matrix
 * @param A the matrix
 * @return a RREF version of A
 */
Matrix rref(const Matrix &A);

/**
 * @return the number on nonzero rows in row echelon form
 */
std::size_t rank(const Matrix &A);

/**
 * @brief Determine rank of matrix in REF
 * @param R Matrix in row-echlon form
 * @return rank of R
 */
std::size_t rank_from_ref(const Matrix &R);
} // namespace la

#endif // LA_ROW_REDUCTION_HPP
