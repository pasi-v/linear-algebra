#include "la/matrix_linear_systems.hpp"
#include "la/row_reduction.hpp"
#include "la/vector_algorithms.hpp"

namespace la {
Matrix augment(const Matrix &A, const Vector &b) {
    const std::size_t m = A.rows();
    const std::size_t n = A.cols();

    if (m != b.size()) {
        throw std::domain_error("Size of b must match number of rows in A");
    }

    Matrix M(m, n + 1);

    for (std::size_t i = 0; i < m; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            M(i, j) = A(i, j);
        }
        M(i, n) = b[i];
    }

    return M;
}

bool is_in_span(const std::vector<Vector> &vectors, const Vector &b) {
    Matrix A = la::from_cols(vectors);
    return is_in_span(A, b);
}

bool is_in_span(const Matrix &A, const Vector &b) {
    if (b.size() != A.rows()) {
        throw std::invalid_argument(
            "Size of b must match the sizes of vectors");
    }

    auto r1 = la::rank(A);
    Matrix Ab = la::augment(A, b);
    auto r2 = la::rank(Ab);

    return r1 == r2;
}

bool is_linear_combination(const Matrix &B,
                           const std::vector<Matrix> &matrices) {
    if (matrices.empty()) {
        throw std::invalid_argument("matrices must not be empty");
    }

    for (std::size_t i = 0; i < matrices.size(); i++) {
        if (!B.has_same_dimensions(matrices.at(i))) {
            throw std::invalid_argument("Sizes of matrices and B must match");
        }
    }

    std::vector<Vector> column_vectors;
    column_vectors.reserve(matrices.size());

    for (std::size_t n = 0; n < matrices.size(); n++) {
        column_vectors.push_back(flatten(matrices.at(n)));
    }

    return is_in_span(column_vectors, flatten(B));
}

bool are_linearly_independent(const std::vector<Matrix> &matrices) {
    if (matrices.empty())
        return true;

    for (std::size_t i = 0; i < matrices.size(); i++) {
        if (!matrices[i].has_same_dimensions(matrices[0])) {
            throw std::invalid_argument("Sizes of matrices must match");
        }
    }

    std::vector<Vector> column_vectors;
    for (const auto &M : matrices) {
        column_vectors.push_back(flatten(M));
    }

    return are_linearly_independent(column_vectors);
}
} // namespace la
