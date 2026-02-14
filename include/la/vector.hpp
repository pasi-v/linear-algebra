#ifndef LA_VECTOR_HPP
#define LA_VECTOR_HPP

#include "utils/utils.hpp"
#include <initializer_list>
#include <ostream>
#include <vector>

namespace la {
/**
 * An n-dimensional vector useful for linear algebra calculations
 */
class Vector {
  public:
    using value_type = double;

    // --- constructors ---
    Vector() = default;

    /** @return a Zero Vector with size s */
    explicit Vector(std::size_t s) : data_(s, 0.0) {}

    explicit Vector(int s)
        : Vector(utils::check_nonnegative(s, "vector size")) {}

    // construct from {1,2,3}
    Vector(std::initializer_list<double> init) : data_(init) {}

    // --- size ---
    /** @return the size (dimensions) of the vector */
    std::size_t size() const { return data_.size(); }

    /** @return whether the vector is empty (length zero) */
    bool empty() const { return data_.empty(); }

    // --- element access ---
    // checked
    double &at(std::size_t i) { return data_.at(i); }
    const double &at(std::size_t i) const { return data_.at(i); }

    // unchecked (match std::vector semantics)
    /** @return the element at i without range check */
    const double &operator[](std::size_t i) const noexcept { return data_[i]; }

    /** @return the element at i without range check */
    double &operator[](std::size_t i) noexcept { return data_[i]; }

    // --- raw data pointers (for std::copy_n etc.) ---
    /** @return the raw data pointet of the vector */
    double *data() noexcept { return data_.data(); }
    /** @return the raw data pointer of the vector */
    const double *data() const noexcept { return data_.data(); }

    // --- iterators (STL-friendly) ---
    std::vector<double>::iterator begin() noexcept { return data_.begin(); }
    std::vector<double>::const_iterator begin() const noexcept {
        return data_.begin();
    }
    std::vector<double>::iterator end() noexcept { return data_.end(); }
    std::vector<double>::const_iterator end() const noexcept {
        return data_.end();
    }

    // --- comparison ---
    /** @return true if the vector elements are the same */
    friend bool operator==(const Vector &a, const Vector &b) {
        return a.data_ == b.data_;
    }

    /** @return true if the vector elements are not the same */
    friend bool operator!=(const Vector &a, const Vector &b) {
        return !(a == b);
    }

    // --- Linear algebra vector operations ---
    /**
     * @return the sum of this and the other vector
     * @throws std::invalid_argument if the vector sizes don't match
     */
    Vector operator+(const Vector &v) const;

    /**
     * @return the difference between this and the other vector
     * @throws std::invalid_argument if the vector sizes don't match
     */
    Vector operator-(const Vector &v) const { return *this + (v * -1); };

    /** @return this vector multiplied by the scalar c */
    Vector operator*(double c) const;

    /**
     * @return a subvector with elements [start, start + length]
     * @throws std::out_of_range if the range is invalid
     */
    Vector subvector(std::size_t start, std::size_t length) const;

    /**
     * @return a subvector from start to end of this vector
     * @throws std::out_of_range if start > size()
     */
    Vector subvector(std::size_t start) const;

    /** @return index of first non-zero element, -1 if all zeroes */
    int first_non_zero_column(double eps = 1e-12) const;

    /** @return first non-zero element, 0 if all zeros */
    double leading_element(double eps = 1e-12) const;

    /** @return true if leading element is 1, false otherwise */
    bool has_leading_one(double eps = 1e-12) const {
        return std::fabs(leading_element(eps) - 1.0) <= eps;
    }

    /**
     * @return true if this is a standard basis vector, i.e. has 1 in one
     * element and the rest are zeros. */
    bool is_standard_basis() const;

    /**
     * @brief head of this vector
     * @param n how many elements to include
     * @return the n-length prefix of the vector
     */
    Vector head(std::size_t n) const;

    /**
     * @param start start index of the suffix
     * @return the suffix of the vector (original without head element)
     *         or an empty Vector if start > this Vector's size
     */
    Vector tail(std::size_t start = 1) const;

  private:
    Vector(std::vector<double>::const_iterator first,
           std::vector<double>::const_iterator last)
        : data_(first, last) {}

    std::vector<double> data_;
};

// --- output ---
inline std::ostream &operator<<(std::ostream &os, const Vector &v) {
    os << "{ ";
    for (std::size_t i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i != v.size() - 1)
            os << ", ";
    }
    os << " }";
    return os;
}
} // namespace la

#endif
