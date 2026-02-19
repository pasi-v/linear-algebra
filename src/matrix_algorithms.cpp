#include "la/matrix_algorithms.hpp"
#include "la/matrix.hpp"
#include "la/pivot_info.hpp"
#include "la/vector.hpp"
#include "la/vector_algorithms.hpp"
#include <cassert>

namespace la {

void row_replace(Matrix &A, std::size_t row, std::size_t lead_col,
                 std::size_t lead_row) {
    const double piv = A(lead_row, lead_col);
    assert(!is_zero_pivot(piv));

    const double a = A(row, lead_col);
    if (is_zero_pivot(a))
        return;

    const double factor = a / piv;

    for (std::size_t col = lead_col; col < A.cols(); ++col) {
        A(row, col) -= factor * A(lead_row, col);
    }

    A(row, lead_col) = 0.0;
}

double determinant(const Matrix &A) {
    if (A.rows() != A.cols()) {
        throw std::domain_error(
            "Determinant is defined only for square matrix");
    }
    if (A.rows() == 1) {
        return A(0, 0);
    }
    if (A.rows() == 2) {
        return A(0, 0) * A(1, 1) - A(0, 1) * A(1, 0);
    }
    if (A.rows() == 3) {
        // Using method 2 from Poole, p. 264
        return A(0, 0) * A(1, 1) * A(2, 2) + A(0, 1) * A(1, 2) * A(2, 0) +
               A(0, 2) * A(1, 0) * A(2, 1) - A(2, 0) * A(1, 1) * A(0, 2) -
               A(2, 1) * A(1, 2) * A(0, 0) - A(2, 2) * A(1, 0) * A(0, 1);
    }
    throw std::domain_error(
        "Determinant is not currently implemented for larger than 3x3 matrix");
}

} // namespace la
