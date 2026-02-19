bool is_in_span(std::vector<Vector> &vectors, Vector &b) {
    Matrix A = la::from_cols(vectors);

    if (b.size() != A.rows()) {
        throw std::invalid_argument(
            "Size of b must match the sizes of vectors");
    }

    auto r1 = la::rank(A);
    Matrix Ab = la::augment(A, b);
    auto r2 = la::rank(Ab);

    return r1 == r2;
}
