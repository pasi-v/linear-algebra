#ifndef ELIMINATED_SYSTEM_HPP
#define ELIMINATED_SYSTEM_HPP

#include "matrix.hpp"
#include "pivot_info.hpp"

namespace la {
/**
 * Result of Gaussian elimination of a linear system Ax = b.
 *
 * Represents the system in row echelon form (REF) together with
 * metadata needed to interpret the solution set.
 *
 * Invariants:
 *  - R is in REF form (not RREF).
 *  - R has n+1 columns, where the last column corresponds to b.
 *  - pivots covers only the variable columns 0..n-1; the RHS column n is
 *    in neither pivot_cols nor free_cols. Hence pivot_cols.size() is
 *    rank(A), and free_cols lists exactly the free variables.
 *  - inconsistent == true iff the system has no solution, i.e. iff
 *    the REF of (A | b) has a pivot in the RHS column.
 */
struct EliminatedSystem {
    Matrix R;          ///< REF of the augmented matrix (A | b)
    PivotInfo pivots;  ///< Pivot and free variable columns of A (not b)
    bool inconsistent; ///< True if the system is inconsistent
};

/**
 * @brief Eliminate a linear system
 * @param A coefficient matrix
 * @param b right-hand side vector if a linear system
 * @return an eliminated system structure
 * @throws std::invalid_argument if the size of b does not match rows of A
 */
EliminatedSystem eliminate_system(const Matrix &A, const Vector &b);
} // namespace la

#endif // ELIMINATED_SYSTEM_HPP
