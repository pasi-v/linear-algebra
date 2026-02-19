#include "la/matrix_transforms.hpp"
#include "la/matrix.hpp"

namespace la {
Matrix transpose(const Matrix &A) {
    std::size_t m = A.rows();
    std::size_t n = A.cols();
    Matrix T(n, m);

    for (std::size_t i = 0; i < m; i++) {
        auto v = A.row(i);
        T.set_col(i, v);
    }

    return T;
}

bool is_symmetric(const Matrix &A) {
    std::size_t m = A.rows();
    std::size_t n = A.cols();

    if (m != n) {
        return false;
    }

    if (m == 1) {
        return true; // Matrix of size 1 is always symmetric
    }

    // No need to look at the diagonal.
    // Examine all A(i,j) of the upper triangle and compare them to the
    // (j, i) of lower triangle.
    for (std::size_t i = 0; i < m; i++) {
        for (std::size_t j = i + 1; j < n; j++) {
            if (A(i, j) != A(j, i)) {
                return false;
            }
        }
    }

    return true;
}
} // namespace la
