#ifndef LINEAR_SYSTEM_HPP
#define LINEAR_SYSTEM_HPP

#include "matrix.hpp"
#include "vector.hpp"

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
 * @brief determine the number of solutions a linear system A|b has
 *
 * @param A coefficient matrix of a linear system
 * @param b right-hand side vector of a linear system
 * @return whether the system has 0, 1 or infinite solutions
 * @throws std::domain_error if the size of b does not match rows of A
 */
SolutionKind n_solutions(const Matrix &A, const Vector &b);

/**
 * @brief solve a linear system A|b
 *
 * @param A coefficient matrix of a linear system
 * @param b right-hand side vector of a linear system
 * @return a solution structure
 * @throws std::domain_error if the size of b does not match rows of A
 */
LinearSystemSolution solve(const Matrix &A, const Vector &b);
} // namespace la

#endif // LINEAR_SYSTEM_HPP
