#include "la/matrix.hpp"
#include "la/matrix_algorithms.hpp"
#include <cassert>

namespace la {

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
