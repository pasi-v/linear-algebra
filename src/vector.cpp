#include "la/vector.hpp"
#include "math_utils/math_utils.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

namespace la {
Vector Vector::operator+(const Vector &v) const {
    if (size() != v.size())
        throw std::invalid_argument("Vector sizes must match for addition");
    Vector result(size());
    for (std::size_t i = 0; i < size(); i++) {
        result[i] = data_[i] + v[i];
    }
    return result;
}

Vector Vector::operator*(double c) const {
    Vector result(size());
    for (std::size_t i = 0; i < size(); i++) {
        result[i] = data_[i] * c;
    }
    return result;
}

Vector Vector::subvector(std::size_t start, std::size_t length) const {
    const std::size_t n = data_.size();

    // Allow an empty slice at end (start==n && length==0), but reject others.
    if (start > n) {
        throw std::out_of_range("Vector::subvector: start > size()");
    }
    // Use n - start to avoid size_t overflow on (start + length).
    if (length > n - start) {
        throw std::out_of_range("Vector::subvector: range exceeds size()");
    }

    Vector out(length);
    if (length > 0) {
        std::copy(data_.begin() + static_cast<std::ptrdiff_t>(start),
                  data_.begin() + static_cast<std::ptrdiff_t>(start + length),
                  out.data_.begin());
    }
    return out; // NRVO / move elision
}

Vector Vector::subvector(std::size_t start) const {
    const std::size_t n = data_.size();
    if (start > n) {
        throw std::out_of_range("Vector::subvector: start > size()");
    }
    return subvector(start, n - start);
}

int Vector::first_non_zero_column(double eps) const {
    for (std::size_t i = 0; i < size(); i++) {
        if (std::fabs((*this)[i]) > eps) {
            return static_cast<int>(i);
        }
    }

    return -1;
}

double Vector::leading_element(double eps) const {
    int column = first_non_zero_column(eps);
    if (column == -1) {
        return 0;
    } else {
        return (*this)[column];
    }
}

bool Vector::is_standard_basis() const {
    bool one_found = false;
    for (std::size_t i = 0; i < size(); i++) {
        auto elem = data_[i];
        if (elem != 0 && elem != 1) {
            return false; // only ones and zeros allowed
        }
        if (elem == 0) {
            continue; // keep on looking
        } else {      // elem is 1, check if we already have found a 1
            if (one_found) {
                return false; // another 1, not allowed
            } else {
                one_found = true;
            }
        }
    }

    return one_found;
}

Vector Vector::head(std::size_t n) const {
    if (n > data_.size()) {
        n = data_.size();
    }
    return Vector(data_.begin(), data_.begin() + n);
}

Vector Vector::tail(std::size_t start) const {
    if (start > data_.size()) {
        return Vector();
    }
    return Vector(data_.begin() + start, data_.end());
}

bool approx_equal(const Vector &a, const Vector &b, double abs_tol,
                  double rel_tol) {
    if (a.size() != b.size())
        return false;

    for (std::size_t i = 0; i < a.size(); ++i) {
        if (!math_utils::nearly_equal(a[i], b[i], abs_tol, rel_tol))
            return false;
    }
    return true;
}
} // namespace la
