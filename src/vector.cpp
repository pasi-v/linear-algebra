#include "la/vector.hpp"
#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <ostream>
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

double Vector::dot_product(const Vector &v) const {
    if (size() != v.size())
        throw std::invalid_argument("Vector sizes must match for dot product");

    double result = 0.0;

    for (std::size_t i = 0; i < size(); i++)
        result += data_[i] * v[i];

    return result;
}

double Vector::angle(const Vector &v, double eps) const {
    // Simple implementation is:
    // return acos(dot_product(v) / (length() * v.length()));
    // But acos needs to be clamped to [-1, 1] without rounding errors drifting
    // it out
    const double asq = this->dot_product(*this);
    const double bsq = v.dot_product(v);

    // Use squared tolerance to avoid an extra sqrt.
    const double tol2 = eps * eps;
    if (asq <= tol2 || bsq <= tol2) {
        throw std::domain_error("angle: undefined for zero-length vector");
    }

    const double denom = std::sqrt(asq * bsq);
    double cos_theta = this->dot_product(v) / denom;

    // Clamp into [-1, 1] (C++11, no std::clamp)
    if (cos_theta > 1.0)
        cos_theta = 1.0;
    else if (cos_theta < -1.0)
        cos_theta = -1.0;

    return std::acos(cos_theta);
};

std::ostream &operator<<(std::ostream &os, const Vector &v) {
    os << "{ ";
    for (std::size_t i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i != v.size() - 1)
            os << ", ";
    }
    os << " }";
    return os;
}

bool Vector::is_zero() const {
    for (std::size_t i = 0; i < size(); i++) {
        if (data_[i] != 0) {
            return false;
        }
    }
    return true;
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
} // namespace la
