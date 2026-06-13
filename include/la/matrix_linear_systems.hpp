#ifndef LA_MATRIX_LINEAR_SYSTEMS_HPP
#define LA_MATRIX_LINEAR_SYSTEMS_HPP

#include "matrix.hpp"
#include "vector.hpp"
#include <vector>

namespace la {
/**
 * @brief construct an augmented matrix from coefficient matrix and righ-hand
 * side vector
 *
 * @param A coefficient matrix of a linear system
 * @param b right-hand side vector of a linear system
 * @return augmented matrix A|b
 * @throws std::invalid_argument if the size of b does not match rows of A
 */
Matrix augment(const Matrix &A, const Vector &b);

/**
 * @brief Construct an augmented matrix from two matrices.
 * @param A left-hand side of the augmented matrix
 * @param B right-hand side of the augmented matrix
 * @return augmented matrix A|B
 * @throws std::invalid_argument if the number of rows do not match
 */
Matrix augment(const Matrix &A, const Matrix &B);

/**
 *  @brief Determine whether b lies in the span of the given vectors
 *  @param vectors
 *  @param b
 *  @return true if there exist scalars c1...cn such that c1v1 + ... + cnvn = b
 *  @throws std::invalid_argument if the vector sizes (including b) don't match
 */
bool is_in_span(const std::vector<Vector> &vectors, const Vector &b);

/**
 *  @brief Determine whether b lies in the span of the given matrix
 *  @param A matrix
 *  @param b vector
 *  @return true if there exist a solution for linear system A|b
 *  @throws std::invalid_argument if the matrix row count doesn't match to
 *          size of b
 */
bool is_in_span(const Matrix &A, const Vector &b);

/**
 * @brief Determine whether B is a linear combination of matrices
 * @param B
 * @param matrices
 * @return true if B is a linear combination of the other matrices, false
 * otherwise
 * @throws std::invalid_argument if the matrix dimensions (including B) don't
 * match, or matrices is empty
 */
bool is_linear_combination(const Matrix &B,
                           const std::vector<Matrix> &matrices);

/**
 * @brief Determine whether a set of matrices are linearly independent
 * @param matrices the matrices to check, sizes must be equal
 * @return true if the matrices are linearly independent or the number of
 * matrices is zero
 * @throws std::invalid_argument if the matrix sizes don't match
 */
bool are_linearly_independent(const std::vector<Matrix> &matrices);
} // namespace la
#endif // LA_MATRIX_LINEAR_SYSTEMS_HPP
