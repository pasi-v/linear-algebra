#include "la/row_reduction.hpp"
#include "la/pivot_info.hpp"
#include "la/vector_algorithms.hpp"
#include <cassert>

namespace la {

struct Pivot {
    std::size_t row, col;
    double value;
};

Pivot find_leftmost_pivot(const Matrix &A, std::size_t start_row);
void normalize_row(Matrix &A, std::size_t row, double pivot_value);
void eliminate_below(Matrix &A, std::size_t lead_row, std::size_t lead_col);
void eliminate_above(Matrix &A, std::size_t lead_row, std::size_t lead_col);
void row_replace(Matrix &A, std::size_t i, std::size_t lead_col,
                 std::size_t lead_row);

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

void normalize_row(Matrix &A, std::size_t row, double pivot_value) {
    if (is_zero_pivot(pivot_value))
        return; // guard
    for (std::size_t j = 0; j < A.cols(); ++j)
        A(row, j) /= pivot_value;
}

void eliminate_below(Matrix &A, std::size_t lead_row, std::size_t lead_col) {
    for (std::size_t i = lead_row + 1; i < A.rows(); ++i) {
        row_replace(A, i, lead_col, lead_row);
    }
}

void eliminate_above(Matrix &A, std::size_t lead_row, std::size_t lead_col) {
    for (std::size_t i = 0; i < lead_row; ++i) {
        row_replace(A, i, lead_col, lead_row);
    }
}

Pivot find_leftmost_pivot(const Matrix &A, std::size_t start_row) {
    std::size_t m = A.rows(), n = A.cols();
    std::size_t best_col = n; // n means “none found”
    for (std::size_t j = 0; j < n; ++j) {
        for (std::size_t i = start_row; i < m; ++i) {
            if (!is_zero_pivot(A(i, j))) {
                best_col = j;
                break;
            }
        }
        if (best_col != n)
            break;
    }
    if (best_col == n)
        return {m, n, 0.0};

    // choose row with largest |value| in best_col
    std::size_t best_row = m;
    double best_abs = 0.0;

    for (std::size_t i = start_row; i < m; ++i) {
        double a = A(i, best_col);
        if (is_zero_pivot(a))
            continue;
        double v = std::fabs(a);
        if (v > best_abs) {
            best_abs = v;
            best_row = i;
        }
    }
    assert(best_row != m);
    return {best_row, best_col, A(best_row, best_col)};
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
            found_zero = is_zero(v);
        } else { // a zero row has been found before this row
            // from now on, it is allowed to have only zero rows
            if (!is_zero(v)) {
                return false;
            }
        }
    }
    // All zero rows are at the bottom.

    // Assert leading entry index grows when going down non-zero vectors
    // and leading entries are all 1.
    int prev_leading_entry_column = -1; // valid columns indexed from 0 to m-1
    for (size_t i = 0; i < A.rows(); i++) {
        Vector v = A.row(i);
        // When we find first zero vector, there will be no more leading
        // enries to check and matrix is in row-echelon form.
        if (is_zero(v)) {
            return true;
        }

        // Assert this leading entry column index is greater than previous.
        int cur_leading_entry_column = first_non_zero_column(v);
        if (cur_leading_entry_column <= prev_leading_entry_column) {
            return false;
        }
        prev_leading_entry_column = cur_leading_entry_column;
    }

    // All requirements hold, the matrix is in row echelon form.
    return true;
}

bool is_rref(const Matrix &A) {
    // 1. Is in row echelon form
    if (!is_ref(A)) {
        return false;
    }

    for (size_t i = 0; i < A.rows(); i++) {
        // 2. The leading entry in each nonzero row is a leading 1
        Vector v = A.row(i);
        if (is_zero(v)) {
            continue; // no leading entry in zero row
        }
        auto leading_elem = leading_element(v);
        if (leading_elem != 1) {
            return false;
        }

        // 3. Each column containing a leading 1 is standard basis vector
        int leading_entry_column = first_non_zero_column(v);
        Vector col = A.column(leading_entry_column);
        if (!is_standard_basis(col)) {
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
    for (std::size_t lead_row = 0; lead_row < m; ++lead_row) {
        // 1. Locate the leftmost non-zero column of the rows below (and
        // including) the top row
        Pivot p = find_leftmost_pivot(R, lead_row);
        if (p.col == n)
            break; // non nonzero columns below => done

        // 2. Create a leading entry in the top row by interchanging it with
        // the top row
        if (p.row != lead_row)
            R.exchange_rows(lead_row, p.row);

        // 3. Use the pivot to create zeros below it on the
        // lead_col.
        eliminate_below(R, lead_row, p.col);
    }

    return R;
}

Matrix rref(const Matrix &A) {
    Matrix R = ref(A); // REF: zeros below pivots, zero rows at bottom

    // Guidelines from Poole, Linear Algebra: A Modern Introduction, 2nd ed, p.
    // 76 Starting from row 2, for each row until first zero row:
    //   - find leftmost pivot column
    //   - create leading one
    //   - create zeros above it by eliminate_above()
    const std::size_t m = R.rows(), n = R.cols();
    for (std::size_t lead_row = 0; lead_row < m; ++lead_row) {
        Pivot p = find_leftmost_pivot(R, lead_row);
        if (p.col == n)
            break; // first zero row => done

        // Normalise the row by pivot value to have leading one
        double pivot_value = R(lead_row, p.col);
        normalize_row(R, lead_row, pivot_value);

        // Use the leading 1 to create zeros above it on the lead column
        eliminate_above(R, lead_row, p.col);
    }

    return R;
}

std::size_t rank(const Matrix &A) {
    Matrix refm = ref(A);
    return rank_from_ref(refm);
}

std::size_t rank_from_ref(const Matrix &R) {
    std::size_t r = 0;
    for (std::size_t i = 0; i < R.rows(); ++i) {
        if (!is_zero(R.row(i))) {
            ++r;
        }
    }
    return r;
}

} // namespace la
