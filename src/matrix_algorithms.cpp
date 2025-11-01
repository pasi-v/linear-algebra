#include "la/matrix_algorithms.hpp"
#include "doctest/doctest.h"
#include "la/matrix.hpp"

namespace la {
struct Pivot {
    std::size_t row, col;
    double value;
};

Pivot find_leftmost_pivot(const Matrix &A, std::size_t start_row);
void normalize_row(Matrix &A, std::size_t row, double pivot_value);
void eliminate_below(Matrix &A, std::size_t lead_row, std::size_t lead_col);

static constexpr double kEps = 1e-12;

Pivot find_leftmost_pivot(const Matrix &A, std::size_t start_row) {
    std::size_t m = A.rows(), n = A.cols();
    std::size_t best_col = n; // n means “none found”
    for (std::size_t j = 0; j < n; ++j) {
        for (std::size_t i = start_row; i < m; ++i) {
            if (std::fabs(A(i, j)) > kEps) {
                best_col = j;
                break;
            }
        }
        if (best_col != n)
            break;
    }
    if (best_col == n)
        return {m, n, 0.0};
    // choose topmost nonzero in best_col
    for (std::size_t i = start_row; i < m; ++i) {
        if (std::fabs(A(i, best_col)) > kEps) {
            return {i, best_col, A(i, best_col)};
        }
    }
    return {m, n, 0.0}; // unreachable logically
}

void normalize_row(Matrix &A, std::size_t row, double pivot_value) {
    if (std::fabs(pivot_value) <= kEps)
        return; // guard
    for (std::size_t j = 0; j < A.cols(); ++j)
        A(row, j) /= pivot_value;
}

void eliminate_below(Matrix &A, std::size_t lead_row, std::size_t lead_col) {
    for (std::size_t i = lead_row + 1; i < A.rows(); ++i) {
        double factor = A(i, lead_col);
        if (std::fabs(factor) <= kEps)
            continue;
        for (std::size_t j = lead_col; j < A.cols(); ++j) {
            A(i, j) -= factor * A(lead_row, j);
        }
    }
}

bool is_ref(const Matrix &A) {
    /*
     * A matrix is in row echelon form if it statisfies the following
     * properties:
     * 1. Any rows consisting entirely of zeros are at the bottom.
     * 2. In each nonzero row, the first nonzero entry (leading entry) is in a
     *    column to the left of any leading entries below it.
     */

    // Assert zero rows are at the bottom:
    // 1. find first zero row starting from top and record its presence
    // 2. if any non-zero rows are found below it, return false
    bool found_zero = false;
    for (size_t i = 0; i < A.rows(); i++) {
        Vector v = A.row(i);
        if (!found_zero) {
            found_zero = v.is_zero();
        } else { // a zero row has been found before this row
            // from now on, it is allowed to have only zero rows
            if (!v.is_zero()) {
                return false;
            }
        }
    }
    // All zero rows are at the bottom.

    // Assert leading entry index grows when going down non-zero vectors
    int prev_leading_entry_column = -1; // valid columns indexed from 0 to m-1
    for (size_t i = 0; i < A.rows(); i++) {
        Vector v = A.row(i);
        // When we find first zero vector, there will be no more leading
        // enries to check and matrix is in row-echelon form.
        if (v.is_zero()) {
            return true;
        }

        // Assert this leading entry column index is greater than previous.
        int cur_leading_entry_column = v.first_non_zero_column();
        if (cur_leading_entry_column <= prev_leading_entry_column) {
            return false;
        }
        prev_leading_entry_column = cur_leading_entry_column;
    }

    // Both requirements hold, the matrix is in row echelon form.
    return true;
}

bool is_rref(const Matrix &A) {
    // 1. Is in row echelon form
    if (!is_ref(A)) {
        std::cout << "Not REF\n";
        return false;
    }

    for (size_t i = 0; i < A.rows(); i++) {
        // 2. The leading entry in each nonzero row is a leading 1
        Vector v = A.row(i);
        if (v.is_zero()) {
            continue; // no leading entry in zero row
        }
        auto leading_element = v.leading_element();
        if (leading_element != 1) {
            std::cout << "Row " << i << " leading element is "
                      << leading_element << "\n";
            return false;
        }

        // 3. Each column containing a leading 1 is standard basis vector
        int leading_entry_column = v.first_non_zero_column();
        Vector col = A.column(leading_entry_column);
        if (!col.is_standard_basis()) {
            std::cout << "Column " << leading_entry_column
                      << " is not standard basis\n";
            return false;
        }
    }

    return true;
}

Matrix ref(const Matrix &A) {
    Matrix R = A; // copy of matrix A

    if (is_ref(R)) {
        return R; // already in REF
    }

    // Guidelines from Poole, Linear Algebra: A Modern Introduction, 2nd ed, pp
    // 72-73
    //
    // For each row as top row, starting with the top row of the whole matrix:
    const std::size_t m = R.rows(), n = R.cols();
    for (std::size_t lead_row = 0; lead_row + 1 < m; ++lead_row) {
        // 1. Locate the leftmost non-zero column of the rows below (and
        // including) the top row
        Pivot p = find_leftmost_pivot(R, lead_row);
        if (p.col == n)
            break; // non nonzero columns below => done

        // 2. Create a leading entry in the top row by interchanging it with
        // the top row
        if (p.row != lead_row)
            R.exchange_rows(lead_row, p.row);

        // Pivot value may have changed after swap
        // 3. Create a leading 1 by scaling the row
        double pivot_value = R(lead_row, p.col);
        normalize_row(R, lead_row, pivot_value);

        // 4. Use the leading 1 to create zeros below it on the
        // lead_col.
        eliminate_below(R, lead_row, p.col);
    }

    return R;
}

std::size_t rank(const Matrix &A) {
    Matrix refm = ref(A);
    std::size_t nonzero_rows = 0;

    for (std::size_t i = 0; i < refm.rows(); i++) {
        if (!(refm.row(i).is_zero())) {
            nonzero_rows++;
        }
    }

    return nonzero_rows;
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

TEST_CASE("is_ref returns true for REF") {
    Matrix m(3, 3, {2, 4, 1, 0, -1, 2, 0, 0, 0});
    CHECK(is_ref(m));
}

TEST_CASE("is_ref returns false for zero row not at the bottom") {
    Matrix m(3, 3, {2, 4, 1, 0, 0, 0, 0, -1, 2}); // zero row at 2
    CHECK(!is_ref(m));
}

TEST_CASE("is_ref returns false for not REF") {
    Matrix m(3, 3, {1, 0, 1, 0, 0, 3, 0, 1, 0});
    CHECK(!is_ref(m));
}

TEST_CASE("is_rref returns true for RREF") {
    Matrix m(5, 7, {1, 2,  0, 0, -3, 1, 0, 0, 0, 1, 0, 4, -1, 0, 0, 0, 0, 1,
                    3, -2, 0, 0, 0,  0, 0, 0, 0, 1, 0, 0, 0,  0, 0, 0, 0});
    CHECK(is_rref(m));
}

TEST_CASE("is_rref returns false for column containing leading 1 not a "
          "standard basis vector") {
    Matrix m(3, 3, {1, 0, 1, 0, 1, 0, 0, 0, 1});
    CHECK(!is_rref(m));
}

TEST_CASE("is_rref returns false for leading entry not 1") {
    Matrix m(3, 3, {2, 0, 0, 0, 1, 0, 0, 0, 1});
    CHECK(!is_rref(m));
}

TEST_CASE("is_rref returns false for not REF") {
    Matrix m(3, 3, {1, 0, 1, 0, 0, 3, 0, 1, 0});
    CHECK(!is_rref(m));
}

TEST_CASE("is_rref returns true for zero matrix") {
    Matrix m(3, 3, 0.0);
    CHECK(is_rref(m));
}

TEST_CASE("is_rref returns true for 1x1 matrix with 0") {
    Matrix m(1, 1, 0.0);
    CHECK(is_rref(m));
}

TEST_CASE("is_rref returns true for 1x1 matrix with 1") {
    Matrix m(1, 1, 1.0);
    CHECK(is_rref(m));
}

TEST_CASE(
    "is_rref returns false for 1x1 matrix with value other than 0 or 1") {
    Matrix m(1, 1, 2.0);
    CHECK(!is_rref(m));
}

TEST_CASE("is_rref returns true for empty matrix") {
    Matrix m(0, 0);
    CHECK(is_rref(m));
}

TEST_CASE("ref returns a matrix that is in REF") {
    Matrix m(4, 5,
             {1, 2, -4, -4, 5, 2, 4, 0, 0, 2, 2, 3, 2, 1, 5, -1, 1, 3, 6, 5});
    // Since REF is not unique, we assert these properties:
    // 1. It is in REF
    // 2. It has same dimensions as original matrix

    Matrix r = ref(m);
    CHECK(is_ref(r));
    CHECK(r.has_same_dimensions(m));
}

TEST_CASE("ref handles correctly a zero row at the beginning") {
    Matrix m(4, 5,
             {0, 0, 0, 0, 0, 2, 4, 0, 0, 2, 2, 3, 2, 1, 5, -1, 1, 3, 6, 5});
    // Since REF is not unique, we assert these properties:
    // 1. It is in REF
    // 2. It has same dimensions as original matrix

    Matrix r = ref(m);
    CHECK(is_ref(r));
    CHECK(r.has_same_dimensions(m));
}

TEST_CASE("ref handles correctly a zero row at the end") {
    Matrix m(4, 5,
             {-1, 1, 3, 6, 5, 2, 4, 0, 0, 2, 2, 3, 2, 1, 5, 0, 0, 0, 0, 0});
    // Since REF is not unique, we assert these properties:
    // 1. It is in REF
    // 2. It has same dimensions as original matrix

    Matrix r = ref(m);
    CHECK(is_ref(r));
    CHECK(r.has_same_dimensions(m));
}

TEST_CASE("ref handles correctly a zero row in the middle") {
    Matrix m(4, 5,
             {-1, 1, 3, 6, 5, 2, 4, 0, 0, 2, 0, 0, 0, 0, 0, 2, 3, 2, 1, 5});
    // Since REF is not unique, we assert these properties:
    // 1. It is in REF
    // 2. It has same dimensions as original matrix

    Matrix r = ref(m);
    CHECK(is_ref(r));
    CHECK(r.has_same_dimensions(m));
}

TEST_CASE("ref handles correctly a zero matrix") {
    Matrix m(3, 3, {0, 0, 0, 0, 0, 0, 0, 0, 0});
    Matrix r = ref(m);
    CHECK(is_ref(r));
    CHECK(r.has_same_dimensions(m));
}

TEST_CASE("ref handles correctly a matrix without zero rows") {
    Matrix m(3, 5, {1, -1, -1, 2, 1, 2, -2, -1, 3, 3, -1, 1, -1, 0, -3});
    Matrix r = ref(m);
    CHECK(is_ref(r));
    CHECK(r.has_same_dimensions(m));
}

TEST_CASE("rank") {
    SUBCASE("zero matrix") {
        Matrix m(2, 2, {0, 0, 0, 0});
        CHECK_EQ(rank(m), 0);
    }

    SUBCASE("rank less than rows") {
        Matrix m(3, 5, {1, -1, -1, 2, 1, 2, -2, -1, 3, 3, -1, 1, -1, 0, -3});
        CHECK_EQ(rank(m), 2);
    }

    SUBCASE("rank equal to rows") {
        Matrix m(4, 5, {1, 2, -4, -4, 5, 2,  4, 0, 0, 2,
                        2, 3, 2,  1,  5, -1, 1, 3, 6, 5});
        CHECK_EQ(rank(m), 4);
    }
}

TEST_CASE("2x2 determinant happy path") {
    Matrix m(2, 2, {1, 2, 3, 4});
    CHECK_EQ(determinant(m), -2);
};

TEST_CASE("determinant of non-square matrix throws") {
    Matrix m(2, 3);
    CHECK_THROWS_AS(determinant(m), std::domain_error);
}

TEST_CASE("determinant of larger than 3x3 matrix throws") {
    Matrix m(4, 4);
    CHECK_THROWS_AS(determinant(m), std::domain_error);
}

TEST_CASE("determinant of 1x1 matrix is its only value") {
    Matrix m(1, 1, 3);
    CHECK_EQ(determinant(m), 3);
}

TEST_CASE("3x3 determinant happy path") {
    Matrix m(3, 3, {5, -3, 2, 1, 0, 2, 2, -1, 3});
    CHECK_EQ(determinant(m), 5);
}
} // namespace la
