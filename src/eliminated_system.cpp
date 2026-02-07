#include "la/eliminated_system.hpp"
#include "la/matrix.hpp"
#include "la/matrix_algorithms.hpp"
#include "la/vector.hpp"
#include "math_utils/math_utils.hpp"

namespace la {

bool is_inconsistent(const Matrix &R, const PivotInfo pivots) {
    std::size_t m = R.rows();
    std::size_t n = R.cols() - 1; // columns of coefficient matrix

    // n = number of variables
    std::size_t rank = pivots.pivot_cols.size();

    // In REF, a system is inconsistent iff there exists a row index i ≥ rank
    // such that the RHS entry of row i is nonzero.
    for (std::size_t i = rank; i < m; i++) {
        if (!math_utils::nearly_equal(R(i, n), 0)) {
            return true;
        }
    }

    return false;
}

EliminatedSystem eliminate_system(const Matrix &A, const Vector &b) {
    Matrix Ab = augment(A, b);
    Matrix R = ref(Ab);
    PivotInfo pivots = find_pivots_and_free_cols(R);
    bool inconsistent = is_inconsistent(R, pivots);
    EliminatedSystem system = {R, pivots, inconsistent};
    return system;
}
} // namespace la
