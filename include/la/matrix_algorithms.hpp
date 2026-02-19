#pragma once
#include "matrix.hpp"
#include "row_reduction.hpp"

namespace la {
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
