#include "la/linear_system.hpp"
#include "la/eliminated_system.hpp"
#include "la/matrix.hpp"
#include "la/matrix_algorithms.hpp"
#include "la/pivot_info.hpp"
#include <cassert>

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
Vector back_substitute_unique(const Matrix &U, const Vector &b) {
    std::size_t n = U.cols();
    Vector x(n);

    for (std::size_t i = n; i-- > 0;) {
        double sum = 0.0;
        for (std::size_t j = i + 1; j < n; ++j) {
            sum += U(i, j) * x[j];
        }
        x[i] = (b[i] - sum) / U(i, i);
    }

    return x;
}

LinearSystemSolution back_substitute_parametric(const Matrix &R,
                                                const PivotInfo &pivots) {
    // Written by ChatGPT 5.2
    const std::size_t n = R.cols() - 1;             // #variables
    const std::size_t r = pivots.pivot_cols.size(); // #pivot rows
    const std::size_t k = pivots.free_cols.size();  // #free vars

    Vector particular(n);                         // assumes zero-init
    std::vector<Vector> directions(k, Vector(n)); // assumes zero-init

    // 1) Initialize free variables:
    //    particular: all free vars = 0
    //    directions: direction j has free var f_j = 1
    for (std::size_t j = 0; j < k; ++j) {
        const std::size_t f = pivots.free_cols[j];
        assert(f < n);
        directions[j][f] = 1.0;
    }

    // 2) Back-substitute pivot variables bottom-up.
    //    Row i has pivot column p = pivots.pivot_cols[i].
    for (std::size_t ii = r; ii-- > 0;) {
        const std::size_t p = pivots.pivot_cols[ii];
        assert(p < n);

        const double piv = R(ii, p);
        assert(!is_zero_pivot(piv)); // or your pivot-eps check

        // Particular: x_p = (b - sum_j>p a_ij x_j) / piv
        double sum_part = 0.0;
        for (std::size_t j = p + 1; j < n; ++j) {
            sum_part += R(ii, j) * particular[j];
        }
        const double rhs = R(ii, n); // augmented column
        particular[p] = (rhs - sum_part) / piv;

        // Directions: x_p = -(sum_j>p a_ij x_j) / piv   (homogeneous)
        for (std::size_t d = 0; d < k; ++d) {
            double sum_dir = 0.0;
            for (std::size_t j = p + 1; j < n; ++j) {
                sum_dir += R(ii, j) * directions[d][j];
            }
            directions[d][p] = -sum_dir / piv;
        }
    }

    return {SolutionKind::Infinite, particular, directions};
}

LinearSystemSolution solve(const Matrix &A, const Vector &b) {
    LinearSystemSolution sol;
    EliminatedSystem es = eliminate_system(A, b);

    if (es.inconsistent) {
        sol.kind = SolutionKind::None;
    }

    else if (es.pivots.free_cols.empty()) {
        sol.kind = SolutionKind::Unique;
        Matrix ref_A = es.R.col_range(0, A.cols());
        Vector ref_b = es.R.column(A.cols());
        Vector x = back_substitute_unique(ref_A, ref_b);
        sol.particular = x;
    }

    else {
        sol.kind = SolutionKind::Infinite;
        auto result = back_substitute_parametric(es.R, es.pivots);
        sol.particular = result.particular;
        sol.directions = result.directions;
    }
    return sol;
}

// This is my old Gauss-Jordan implementation, which I have replaced
// by the more efficient Gaussian elimination in solve().
// Keeping this as a reminder for how Gauss-Jordan can be implemented.
LinearSystemSolution solve_gauss_jordan(const Matrix &A, const Vector &b) {
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
