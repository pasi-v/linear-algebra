#include "la/matrix_transforms.hpp"
#include "la/approx.hpp"
#include "la/matrix.hpp"
#include "la/matrix_linear_systems.hpp"
#include "la/row_reduction.hpp"
#include "math_utils/math_utils.hpp"

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
            if (!math_utils::nearly_equal(A(i, j), A(j, i))) {
                return false;
            }
        }
    }

    return true;
}

bool inverse(const Matrix &in, Matrix &out) {
    std::size_t n = in.rows();

    if (in.cols() != n) {
        throw std::invalid_argument("The matrix in must be square");
    }

    // This is not the most performant way to calculate inverse,
    // but this matches Poole section 3.3. Gauss-Jordan method.
    Matrix augmented = augment(in, identity(n));
    Matrix reduced = rref(augmented);
    Matrix left = reduced.col_range(0, n);

    // Check that left is identity
    if (approx_equal(left, identity(n), math_utils::kDefaultAbsTol,
                     math_utils::kDefaultRelTol)) {
        // Extract right-hand side to result
        out = reduced.col_range(n, n * 2);
        return true;
    }

    return false;
}
} // namespace la
