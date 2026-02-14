#include "la/vector.hpp"
#include "math_utils/math_utils.hpp"
#include <algorithm>
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

Vector Vector::operator-(const Vector &v) const {
    if (size() != v.size())
        throw std::invalid_argument("Vector sizes must match for addition");
    Vector result(size());
    for (std::size_t i = 0; i < size(); i++) {
        result[i] = data_[i] - v[i];
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
