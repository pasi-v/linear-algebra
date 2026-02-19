#ifndef LA_DETERMINANT_HPP
#define LA_DETERMINANT_HPP

#include "matrix.hpp"

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
} // namespace la

#endif // LA_DETERMINANT_HPP
