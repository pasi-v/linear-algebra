#include "la/eliminated_system.hpp"
#include "la/matrix.hpp"
#include "la/matrix_algorithms.hpp"
#include "la/vector.hpp"

namespace la {

EliminatedSystem eliminate_system(const Matrix &A, const Vector &b) {
    Matrix Ab = augment(A, b);
    std::size_t nAb = Ab.cols() - 1;
    RefResult result = ref(Ab);
    // Remove the last column from whichever list in pivotinfo it ended up,
    // because in linear system only columns 0...n-1 are variables.
    bool inconsistent = false;
    if (!result.pivots.pivot_cols.empty() &&
        result.pivots.pivot_cols.back() == nAb) {
        result.pivots.pivot_cols.pop_back();
        inconsistent = true;
    } else if (!result.pivots.free_cols.empty() &&
               result.pivots.free_cols.back() == nAb) {
        result.pivots.free_cols.pop_back();
    }

    EliminatedSystem system = {result.R, result.pivots, inconsistent};
    return system;
}
} // namespace la
