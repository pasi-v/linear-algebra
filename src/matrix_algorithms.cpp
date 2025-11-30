#include "la/matrix_algorithms.hpp"
#include "la/matrix.hpp"
#include "la/vector.hpp"
#include <cmath>

namespace la {

inline bool is_zero(double x) {
    return std::abs(x) < kEps;
}

struct Pivot {
    std::size_t row, col;
    double value;
};

struct PivotInfo {
    std::vector<std::size_t> pivot_cols;
    std::vector<std::size_t> free_cols;
};

Pivot find_leftmost_pivot(const Matrix &A, std::size_t start_row);
PivotInfo find_pivots_and_free_cols(const Matrix& R);
void normalize_row(Matrix &A, std::size_t row, double pivot_value);
void eliminate_below(Matrix &A, std::size_t lead_row, std::size_t lead_col);
void eliminate_above(Matrix &A, std::size_t lead_row, std::size_t lead_col);
bool almost_zero(double value);
std::size_t rank_from_ref(const Matrix &R);
Vector back_substitute_unique(const Matrix &A, const Vector &b);
Vector extract_unique(const Matrix &R);
LinearSystemSolution extract_parametric(const Matrix &R);

void row_replace(Matrix &A, std::size_t i, std::size_t lead_col,
                 std::size_t lead_row);

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

// R is in RREF and pivots are ordered from top left to bottom right.
PivotInfo find_pivots_and_free_cols(const Matrix& R) {
    const std::size_t m = R.rows();
    const std::size_t n = R.cols() - 1;  // number of variables
    const std::size_t r = rank(R);       // rank(A) == number of pivot rows

    PivotInfo info;
    info.pivot_cols.reserve(r);
    info.free_cols.reserve(n - r);

    std::size_t pivot_row = 0;

    for (std::size_t col = 0; col < n; ++col) {
        // We are scanning left-to-right; for each variable column,
        // either this column is the pivot for pivot_row, or it’s free.
        if (pivot_row < r && !is_zero(R(pivot_row, col))) {
            // This column has the pivot for this row
            info.pivot_cols.push_back(col);
            ++pivot_row;
        } else {
            // No pivot here -> free variable column
            info.free_cols.push_back(col);
        }
    }

    return info;
}

void normalize_row(Matrix &A, std::size_t row, double pivot_value) {
    if (std::fabs(pivot_value) <= kEps)
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

bool almost_zero(double value) { return std::fabs(value) <= kEps; }

std::size_t rank_from_ref(const Matrix &R) {
    std::size_t r = 0;
    for (std::size_t i = 0; i < R.rows(); ++i) {
        if (!R.row(i).is_zero()) {
            ++r;
        }
    }
    return r;
}

void row_replace(Matrix &A, std::size_t i, std::size_t lead_col,
                 std::size_t lead_row) {
    double factor = A(i, lead_col);
    if (std::fabs(factor) <= kEps) {
        return;
    };
    for (std::size_t j = lead_col; j < A.cols(); ++j) {
        A(i, j) -= factor * A(lead_row, j);
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
    // and leading entries are all 1.
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

        double leading_entry = A(i, cur_leading_entry_column);
        if (!almost_zero(1.0 - leading_entry)) {
            return false;
        }
    }

    // All requirements hold, the matrix is in row echelon form.
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

Matrix rref(const Matrix &A) {
    Matrix R = ref(A); // REF: pivots are 1, zeros below, zero rows at bottom

    // Guidelines from Poole, Linear Algebra: A Modern Introduction, 2nd ed, p.
    // 76 Starting from row 2, for each row until first zero row:
    //   - find leftmost pivot column (leading 1)
    //   - create zeros above it by eliminate_above()
    const std::size_t m = R.rows(), n = R.cols();
    for (std::size_t lead_row = 0; lead_row < m; ++lead_row) {
        Pivot p = find_leftmost_pivot(R, lead_row);
        if (p.col == n)
            break; // first zero row => done

        // Use the leading 1 to create zeros above it on the lead column
        eliminate_above(R, lead_row, p.col);
    }

    return R;
}

std::size_t rank(const Matrix &A) {
    Matrix refm = ref(A);
    return rank_from_ref(refm);
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

SolutionKind n_solutions(const Matrix &A, const Vector &b) {
    std::size_t m = A.rows();
    std::size_t n = A.cols();

    Matrix Ab = augment(A, b);
    Matrix R = ref(Ab);
    Matrix ref_A = R.col_range(0, A.cols());
    Vector ref_b = R.column(R.cols() - 1);

    // Find inconsistency, which is that a row of A in REF is all zeroes
    // and corresponding element in b is non-zero:
    // [0 ... 0 | c ] with c != 0:
    for (std::size_t i = 0; i < m; i++) {
        if (ref_A.row(i).is_zero() && !almost_zero(ref_b.at(i))) {
            return SolutionKind::None;
        }
    }

    std::size_t rankA = rank_from_ref(ref_A);
    const std::size_t n_free_variables = (n > rankA) ? (n - rankA) : 0;

    if (n_free_variables == 0) {
        // We have already established that system is consistent to zero
        // free variables means unique solution
        return SolutionKind::Unique;
    } else if (n_free_variables > 0) {
        // consistent and infinite solutions
        return SolutionKind::Infinite;
    }

    // Should never reach this point, just keeping compiler happy
    return SolutionKind::None;
}

// This is for Gaussian elimination with unique solution from REF.
// With Gauss-Jordan and RREF there is no need for back substition, so no
// point in extending this to handle infinite solutions.
Vector back_substitute_unique(const Matrix &A, const Vector &b) {
    std::size_t n = A.cols();  // assume square + unique solution
    Vector x(n);

    for (int i = n - 1; i >= 0; --i) {
        double sum = A.row(i).tail(i+1).dot_product(x.tail(i+1));
        x[i] = b[i] - sum;
    }

    return x;
}

Vector extract_unique(const Matrix &R) {
    // My implementation of RREF guarantees that the pivot columns are in
    // increasing order.
    // R is of form A|b.  Thus R.cols() == n + 1 and R.rows() >= n where
    // n is the rank.
    // Therefore for unique solution, this approach is safe:
    std::size_t n = R.cols() - 1;  // number of variables
    return R.column(n).head(n);
}


LinearSystemSolution extract_parametric(const Matrix &R) {
    // 1. Detect free columns = non-pivot columns.
    // 2. For each free column j_f:
    //     create a direction vector dir (set dir[j_f]=1).
    // for each pivot row r with pivot column c:
    //     dir[c] = -RREF(r, j_f);
    // 3. For the particular solution:
    //     set all free variables to zero;
    //     set pivot variables to RREF(r, last_column).
    const std::size_t m = R.rows();
    const std::size_t n = R.cols() - 1;  // number of variables
    const std::size_t r = rank(R);       // rank(A)

    LinearSystemSolution sol;
    sol.kind = SolutionKind::Infinite;
    sol.particular = Vector(n);          // zero vector
    sol.directions.clear();

    PivotInfo piv = find_pivots_and_free_cols(R);

    // --- 1. Particular solution: free vars = 0, pivot vars from RHS ---
    // last column index:
    const std::size_t rhs_col = n;

    Vector x(n); // zeroes
    for (std::size_t i = 0; i < r; ++i) {
        std::size_t c = piv.pivot_cols[i];  // variable index for this pivot row
        x[c] = R(i, rhs_col);
    }
    sol.particular = x;

    // --- 2. Directions: null space basis, one per free variable ---
    sol.directions.reserve(piv.free_cols.size());

    for (std::size_t k = 0; k < piv.free_cols.size(); ++k) {
        std::size_t free_col = piv.free_cols[k];

        Vector dir(n);       // start with all zeros
        dir[free_col] = 1.0; // this parameter is "1", others "0"

        // For each pivot row, express pivot variable in terms of this free variable
        for (std::size_t i = 0; i < r; ++i) {
            std::size_t pivot_col = piv.pivot_cols[i];
            double coeff = R(i, free_col);  // coefficient of this free var in row i

            // In RREF, row i equation is:
            // x_pivot + sum_j R(i, j) * x_j = RHS
            // For homogeneous system A*n = 0: x_pivot = - sum_j R(i, j) * x_j
            dir[pivot_col] = -coeff;
        }

        sol.directions.push_back(std::move(dir));
    }

    return sol;
}


LinearSystemSolution solve(const Matrix &A, const Vector &b) {
    LinearSystemSolution sol;
    // TODO: n_solutions calculates ref too
    sol.kind = n_solutions(A, b);

    Matrix Ab = augment(A, b);
    Matrix R = rref(Ab);

    if (sol.kind == SolutionKind::None) {
        return sol;
    }

    if (sol.kind == SolutionKind::Unique) {
        Vector x = extract_unique(R);
        sol.particular = x;
        return sol;
    }

    auto result = extract_parametric(R);
    sol.particular = result.particular;
    sol.directions = result.directions;
    return sol;
}
} // namespace la
