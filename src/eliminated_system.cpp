#include "la/eliminated_system.hpp"
#include "la/matrix.hpp"
#include "la/matrix_algorithms.hpp"
#include "la/vector.hpp"

namespace la {

EliminatedSystem eliminate_system(const Matrix &A, const Vector &b) {
    Matrix Ab = augment(A, b);
    std::size_t nAb = Ab.cols() - 1;
    RefResult result = ref(Ab);
    Matrix R = result.R;
    // Remove the last column from whichever list in pivotinfo it ended up,
    // because in linear system only columns 0...n-1 are variables.
    PivotInfo pivots = result.pivots;
    bool inconsistent = false;
    if (!pivots.pivot_cols.empty() && pivots.pivot_cols.back() == nAb) {
        pivots.pivot_cols.pop_back();
        inconsistent = true;
    } else if (!pivots.free_cols.empty() && pivots.free_cols.back() == nAb) {
        pivots.free_cols.pop_back();
    }

    EliminatedSystem system = {R, pivots, inconsistent};
    return system;
}
} // namespace la
