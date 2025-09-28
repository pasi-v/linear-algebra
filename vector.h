#include "utils.h"
#include <initializer_list>
#include <vector>

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
   * @return the dot product betwee this and the other vector
   * @throws std::invalid_argument if the vector sizes don't match
   */
  double dot_product(const Vector &v) const;

  /** @return the length (magnitude) of this vector */
  double length() const { return std::sqrt(dot_product(*this)); }

  /**
   * @return the magnitude of the difference of the vectors
   * @throws std::invalid_argument if the vector sizes don't match
   */
  double difference(const Vector &v) const { return (*this - v).length(); };

  /**
   * @return the angle between the vectors in radians
   * @throws std::invalid_argument if the vector sizes don't match
   * @throws std::domain_error if either vector is zero length within eps
   */
  double angle(const Vector &v, double eps = 1e-12) const;

  /**
   * @return the projection of v onto this vector
   * @throws std::invalid_argument if the vector sizes don't match
   */
  Vector proj(const Vector &v) const {
    return *this * (dot_product(v) / dot_product(*this));
  };

  /** @return true if all elements are zero, false otherwise */
  bool is_zero() const;

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

private:
  std::vector<double> data_;
};
