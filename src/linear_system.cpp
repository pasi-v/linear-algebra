#include "la/linear_system.hpp"
#include "la/eliminated_system.hpp"
#include "la/matrix.hpp"
#include "la/matrix_algorithms.hpp"
#include "la/pivot_info.hpp"

namespace la {
Vector back_substitute_unique(const Matrix &A, const Vector &b);
LinearSystemSolution extract_parametric(const Matrix &R);
Vector extract_unique(const Matrix &R);

LinearSystemSolution extract_parametric(const Matrix &R) {
    // 1. Detect free columns = non-pivot columns.
    // 2. For each free column j_f:
    //     create a direction vector dir (set dir[j_f]=1).
    // for each pivot row r with pivot column c:
    //     dir[c] = -RREF(r, j_f);
    // 3. For the particular solution:
    //     set all free variables to zero;
    //     set pivot variables to RREF(r, last_column).
    const std::size_t n = R.cols() - 1; // number of variables
    const std::size_t r = rank(R);      // rank(A)

    LinearSystemSolution sol;
    sol.kind = SolutionKind::Infinite;
    sol.particular = Vector(n); // zero vector
    sol.directions.clear();

    PivotInfo piv = find_pivots_and_free_cols(R);

    // --- 1. Particular solution: free vars = 0, pivot vars from RHS ---
    // last column index:
    const std::size_t rhs_col = n;

    Vector x(n); // zeroes
    for (std::size_t i = 0; i < r; ++i) {
        std::size_t c = piv.pivot_cols[i]; // variable index for this pivot row
        x[c] = R(i, rhs_col);
    }
    sol.particular = x;

    // --- 2. Directions: null space basis, one per free variable ---
    sol.directions.reserve(piv.free_cols.size());

    for (std::size_t k = 0; k < piv.free_cols.size(); ++k) {
        std::size_t free_col = piv.free_cols[k];

        Vector dir(n);       // start with all zeros
        dir[free_col] = 1.0; // this parameter is "1", others "0"

        // For each pivot row, express pivot variable in terms of this free
        // variable
        for (std::size_t i = 0; i < r; ++i) {
            std::size_t pivot_col = piv.pivot_cols[i];
            double coeff =
                R(i, free_col); // coefficient of this free var in row i

            // In RREF, row i equation is:
            // x_pivot + sum_j R(i, j) * x_j = RHS
            // For homogeneous system A*n = 0: x_pivot = - sum_j R(i, j) * x_j
            dir[pivot_col] = -coeff;
        }

        sol.directions.push_back(std::move(dir));
    }

    return sol;
}

Vector extract_unique(const Matrix &R) {
    // My implementation of RREF guarantees that the pivot columns are in
    // increasing order.
    // R is of form A|b.  Thus R.cols() == n + 1 and R.rows() >= n where
    // n is the rank.
    // Therefore for unique solution, this approach is safe:
    std::size_t n = R.cols() - 1; // number of variables
    return R.column(n).head(n);
}

SolutionKind n_solutions(const Matrix &A, const Vector &b) {
    EliminatedSystem es = eliminate_system(A, b);

    if (es.inconsistent == true) {
        return SolutionKind::None;
    }

    std::size_t n = A.cols();

    Matrix ref_A = es.R.col_range(0, A.cols());

    std::size_t rankA = rank_from_ref(ref_A);
    const std::size_t n_free_variables = (n > rankA) ? (n - rankA) : 0;

    if (n_free_variables == 0) {
        // We have already established that system is consistent to zero
        // free variables means unique solution
        return SolutionKind::Unique;
    } else if (n_free_variables > 0) {
        // consistent and infinite solutions
        return SolutionKind::Infinite;
    }

    // Should never reach this point, just keeping compiler happy
    return SolutionKind::None;
}

// This is for Gaussian elimination with unique solution from REF.
// With Gauss-Jordan and RREF there is no need for back substition, so no
// point in extending this to handle infinite solutions.
Vector back_substitute_unique(const Matrix &A, const Vector &b) {
    std::size_t n = A.cols(); // assume square + unique solution
    Vector x(n);

    for (int i = n - 1; i >= 0; --i) {
        double sum = A.row(i).tail(i + 1).dot_product(x.tail(i + 1));
        x[i] = b[i] - sum;
    }

    return x;
}

LinearSystemSolution solve(const Matrix &A, const Vector &b) {
    LinearSystemSolution sol;
    EliminatedSystem es = eliminate_system(A, b);

    if (es.inconsistent) {
        sol.kind = SolutionKind::None;
    }

    else if (es.pivots.free_cols.empty()) {
        sol.kind = SolutionKind::Unique;
        Vector x = extract_unique(es.R);
        sol.particular = x;
    }

    else {
        sol.kind = SolutionKind::Infinite;
        auto result = extract_parametric(es.R);
        sol.particular = result.particular;
        sol.directions = result.directions;
    }
    return sol;
}
} // namespace la
