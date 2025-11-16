#pragma once
#include "matrix.hpp"

namespace la {
enum class SolutionKind {
    None,    // no solutions (inconsistent system)
    Unique,  // exactly one solution
    Infinite // infinitely many solutions
};

struct LinearSystemSolution {
    SolutionKind kind;

    // Defined iff kind != SolutionKind::None
    Vector particular;

    // Basis for the homogeneous solution space.
    // - empty if unique solution
    // - size() == nullity if infinite solutions
    std::vector<Vector> directions;

    bool has_solution() const { return kind != SolutionKind::None; }
    bool is_unique() const { return kind == SolutionKind::Unique; }
    bool is_infinite() const { return kind == SolutionKind::Infinite; }
};

/**
 * @brief Numerical tolerance used for floating-point comparisons.
 *
 * Values with absolute magnitude less than or equal to `kEps` are
 * considered to be zero in equality checks and elimination steps.
 *
 * This constant defines the default threshold for detecting pivots,
 * zeroing entries, and other floating-point comparisons in matrix
 * operations such as REF/RREF transformations.
 *
 * @note Users may rely on this value when implementing custom
 *       algorithms to maintain consistency with the library’s
 *       numerical behavior.
 */
static constexpr double kEps = 1e-12;

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
 * @brief return a row echelon form of this matrix
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
 * @brief determine the number of solutions a linear system A|b has
 *
 * @param A coefficient matrix of a linear system
 * @param b right-hand side vector of a linear system
 * @return whether the system has 0, 1 or infinite solutions
 * @throws std::domain_error if the size of b does not match rows of A
 */
SolutionKind n_solutions(const Matrix &A, const Vector &b);
} // namespace la
