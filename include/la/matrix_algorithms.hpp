#pragma once
#include "matrix.hpp"

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

/**
 * @brief calculate the determinant of this matrix if the matrix is at most
 * 3x3
 *
 * The limit to 3x3 is temporary and will be extended to nxn
 * matrices in the future.
 *
 * @return the determinant of this matrix
 * @throws std::domain_error if the matrix is larger than 3x3 or not square
 * matrix
 */
double determinant(const Matrix &A);

/**
 * @brief construct an augmented matrix from coefficient matrix and righ-hand
 * side vector
 *
 * @param A coefficient matrix of a linear system
 * @param b right-hand side vector of a linear system
 * @return augmented matrix A|b
 * @throws std::domain_error if the size of b does not match rows of A
 */
Matrix augment(const Matrix &A, const Vector &b);

/**
 * @brief transpose
 * @param A matrix to transpose
 * @return transposed matrix
 */
Matrix transpose(const Matrix &A);

/**
 * @brief is A symmetric
 * @param A matrix to examine
 * @return true if A is symmetric, false otherwise
 */
bool is_symmetric(const Matrix &A);
} // namespace la
