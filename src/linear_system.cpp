#include "la/linear_system.hpp"
#include "la/eliminated_system.hpp"
#include "la/matrix.hpp"
#include "la/matrix_algorithms.hpp"
#include "la/pivot_info.hpp"
#include <stdexcept>

namespace la {
Vector back_substitute_unique(const Matrix &A, const Vector &b);

SolutionKind n_solutions(const Matrix &A, const Vector &b) {
    EliminatedSystem es = eliminate_system(A, b);

    if (es.inconsistent == true) {
        return SolutionKind::None;
    }

    return es.pivots.free_cols.empty() ? SolutionKind::Unique
                                       : SolutionKind::Infinite;
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
        // reasoning for b[i] being correct is far away, use at()
        x[i] = (b.at(i) - sum) / U(i, i);
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
        if (f >= n)
            throw std::out_of_range(
                "back_substitute_parametric: free column index out of range");
        directions[j][f] = 1.0;
    }

    // 2) Back-substitute pivot variables bottom-up.
    //    Row i has pivot column p = pivots.pivot_cols[i].
    for (std::size_t ii = r; ii-- > 0;) {
        const std::size_t p = pivots.pivot_cols[ii];
        if (p >= n)
            throw std::out_of_range(
                "back_substitute_parametric: pivot column index out of range");

        const double piv = R(ii, p);
        if (is_zero_pivot(piv))
            throw std::invalid_argument(
                "back_substitute_parametric: zero pivot encountered");

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
} // namespace la
