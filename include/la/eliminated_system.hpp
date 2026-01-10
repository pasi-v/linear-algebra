#ifndef ELIMINATED_SYSTEM_HPP
#define ELIMINATED_SYSTEM_HPP

#include "matrix.hpp"
#include "pivot_info.hpp"

namespace la {
/**
 * Result of Gaussian / Gauss–Jordan elimination of a linear system Ax = b.
 *
 * Represents the system in reduced row echelon form (RREF) together with
 * metadata needed to interpret the solution set.
 *
 * Invariants:
 *  - R is in RREF form.
 *  - R has n+1 columns, where the last column corresponds to b.
 *  - pivots describes exactly the pivot and free columns of R.
 *  - inconsistent == true iff the system has no solution.
 */
struct EliminatedSystem {
    Matrix R;          ///< RREF of the augmented matrix (A | b)
    PivotInfo pivots;  ///< Pivot and free column information for R
    bool inconsistent; ///< True if the system is inconsistent
};

/**
 * @brief Eliminate a linear system
 * @param A coefficient matrix
 * @param b right-hand side vector if a linear system
 * @return an eliminated system structure
 * @throws std::domain_error if the size of b does not match rows of A
 */
EliminatedSystem eliminate_system(const Matrix &A, const Vector &b);
} // namespace la

#endif // ELIMINATED_SYSTEM_HPP
