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

} // namespace la

#endif // LA_MATRIX_TRANSFORMS_HPP
