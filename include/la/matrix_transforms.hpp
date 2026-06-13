#ifndef LA_MATRIX_TRANSFORMS_HPP
#define LA_MATRIX_TRANSFORMS_HPP

#include "matrix.hpp"

namespace la {
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

/**
 * @brief inverse calculate inverse of matrix if possible
 * @param in the matrix whose inverse to calculate
 * @param out the inverse, if the matrix in was invertible
 * @return true if matrix in was invertible
 * @throws std::invalid_argument if in is not a square matrix
 */
bool inverse(const Matrix &in, Matrix &out);

} // namespace la

#endif // LA_MATRIX_TRANSFORMS_HPP
