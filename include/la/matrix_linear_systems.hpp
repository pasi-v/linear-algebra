#ifndef LA_MATRIX_LINEAR_SYSTEMS_HPP
#define LA_MATRIX_LINEAR_SYSTEMS_HPP

#include "matrix.hpp"

namespace la {
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
 *  @brief Determine whether b lies in the span of the given vectors
 *  @param vectors
 *  @param b
 *  @return true if there exist scalars c1...cn such that c1v1 + ... + cnvn = b
 *  @throws std::invalid_argument if the vector sizes (including b) don't match
 */
bool is_in_span(std::vector<Vector> &vectors, Vector &b);

} // namespace la
#endif // LA_MATRIX_LINEAR_SYSTEMS_HPP
