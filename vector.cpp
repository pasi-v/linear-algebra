#include "vector.h"
#include "doctest.h"
#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <ostream>
#include <stdexcept>
#include <vector>

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
    } else {    // elem is 1, check if we already have found a 1
      if (one_found) {
        return false; // another 1, not allowed
      } else {
        one_found = true;
      }
    }
  }

  return one_found;
}

TEST_CASE("Vector()") {
  int size = 3;
  Vector v(size);
  CHECK(v.size() == size);
}

TEST_CASE("Construct vector of size 0 does not throw") {
  int size = 0;
  Vector v(size);
  CHECK(v.size() == size);
}

TEST_CASE("Empty vector is empty") {
  Vector v;
  CHECK(v.empty());
}

TEST_CASE("Non-empty vector is not empty") {
  Vector v(1);
  CHECK(!v.empty());
}

TEST_CASE("Subscript operator happy path") {
  int size = 3;
  Vector v(size);
  v[0] = 1;
  CHECK(v.at(0) == 1);
}

TEST_CASE("Subscript operator negative throws") {
  int size = 3;
  Vector v(size);
  CHECK_THROWS_AS(v.at(-1) = 1, std::out_of_range);
}

TEST_CASE("Subscript operator equal to size throws") {
  int size = 3;
  Vector v(size);
  CHECK_THROWS_AS(v.at(size) = 1, std::out_of_range);
}

TEST_CASE("Subscript operator larger than size throws") {
  int size = 3;
  Vector v(size);
  CHECK_THROWS_AS(v.at(size + 1) = 1, std::out_of_range);
}

TEST_CASE("Vector comparison") {
  Vector u{1.0, 2.0};
  Vector v{1.0, 2.0};
  Vector w{1.0, 3.0};

  CHECK(u == v);
  CHECK_FALSE(u == w);
}

TEST_CASE("Vector addition happy path") {
  Vector u{1, 2};
  Vector v{2, 2};
  Vector sum = u + v;
  Vector expected{3, 4};
  CHECK(sum == expected);
}

TEST_CASE("Vector addition different sizes throws") {
  Vector u(2);
  Vector v(3);
  CHECK_THROWS_AS(u + v, std::invalid_argument);
}

TEST_CASE("Vector scalar multiplication happy path") {
  int c = 2;
  Vector v{-2, 4};
  Vector cv = v * c;
  Vector expected{-4, 8};
  CHECK(cv == expected);
}

TEST_CASE("Vector subtraction happy path") {
  Vector u{1, 2};
  Vector v{2, 2};
  Vector diff = u - v;
  Vector expected{-1, 0};
  CHECK(diff == expected);
}

TEST_CASE("Vector subtraction different sizes throws") {
  Vector u(2);
  Vector v(3);
  CHECK_THROWS_AS(u + v, std::invalid_argument);
}

TEST_CASE("Vector::subvector basics") {
  Vector v{10, 20, 30, 40, 50};

  SUBCASE("middle segment") {
    auto s = v.subvector(1, 3); // {20, 30, 40}
    CHECK(s.size() == 3);
    CHECK(s[0] == doctest::Approx(20));
    CHECK(s[1] == doctest::Approx(30));
    CHECK(s[2] == doctest::Approx(40));
  }

  SUBCASE("to end") {
    auto s = v.subvector(2); // {30, 40, 50}
    CHECK(s.size() == 3);
    CHECK(s[0] == doctest::Approx(30));
    CHECK(s[2] == doctest::Approx(50));
  }

  SUBCASE("empty at end") {
    auto s = v.subvector(5, 0);
    CHECK(s.size() == 0);
  }

  SUBCASE("start > size throws") {
    CHECK_THROWS_AS(v.subvector(6, 0), std::out_of_range);
  }

  SUBCASE("range exceeds size throws") {
    CHECK_THROWS_AS(v.subvector(4, 2), std::out_of_range);
  }
}

TEST_CASE("Dot product happy case") {
  Vector u{1, 2, -3};
  Vector v{-3, 5, 2};
  double result = u.dot_product(v);
  CHECK(result == 1.0);
}

TEST_CASE("Dot product different sizes throws") {
  Vector u(2);
  Vector v(3);
  CHECK_THROWS_AS(u.dot_product(v), std::invalid_argument);
}

TEST_CASE("Vector length") {
  Vector u{2, 3};
  CHECK(u.length() == std::sqrt(13.0));
}

TEST_CASE("Vector difference happy path") {
  Vector u{std::sqrt(2), 1, -1};
  Vector v{0, 2, -2};
  CHECK(u.difference(v) == 2.0);
}

TEST_CASE("Difference different sizes throws") {
  Vector u(2);
  Vector v(3);
  CHECK_THROWS_AS(u.difference(v), std::invalid_argument);
}

TEST_CASE("Angle happy path") {
  Vector u{2, 1, -2};
  Vector v{1, 1, 1};
  double result = u.angle(v);
  CHECK(result == doctest::Approx(1.377).epsilon(0.01));
}

TEST_CASE("Angle different sizes throws") {
  Vector u{1, 2};
  Vector v{1, 2, 3};
  CHECK_THROWS_AS(u.angle(v), std::invalid_argument);
}

TEST_CASE("Angle with zero vector throws") {
  Vector u{0, 0, 0};
  Vector v{1, 2, 3};
  CHECK_THROWS_AS(u.angle(v), std::domain_error);
}

TEST_CASE("Projection happy path") {
  Vector u{2, 1};
  Vector v{-1, 3};
  Vector result = u.proj(v);
  Vector expected{2.0 / 5.0, 1.0 / 5.0};
  CHECK(result == expected);
}

TEST_CASE("Projection different sizes throws") {
  Vector u(2);
  Vector v(3);
  CHECK_THROWS_AS(u.proj(v), std::invalid_argument);
}

TEST_CASE("is_zero returns TRUE for zero vector") {
  Vector v{0, 0, 0};
  CHECK(v.is_zero());
}

TEST_CASE("is_zero returns FALSE for non-zero element in vector") {
  Vector v{0, 3, 0};
  CHECK(!v.is_zero());
}

TEST_CASE("first_non_zero_column returns 0 if first element is non-zero") {
  Vector v{1, 2, 3};
  CHECK_EQ(0, v.first_non_zero_column());
}

TEST_CASE(
    "first_non_zero_column returns m-1 if only last element is non-zero") {
  Vector v{0, 0, 1};
  CHECK_EQ(2, v.first_non_zero_column());
}

TEST_CASE("first_non_zero_column returns -1 for zero vector") {
  Vector v{0, 0, 0};
  CHECK_EQ(-1, v.first_non_zero_column());
}

TEST_CASE("leading_element returns first element if it is not zero") {
  Vector v{1, 2, 3};
  CHECK_EQ(1, v.leading_element());
}

TEST_CASE("leading_element returns last element if preceding ones are zero") {
  Vector v{0, 0, 4};
  CHECK_EQ(4, v.leading_element());
}

TEST_CASE("leading_element returns 0 for zero vector") {
  Vector v{0, 0, 0};
  CHECK_EQ(0, v.leading_element());
}

TEST_CASE("has_leading_one returns true if leading element is 1") {
  Vector v{0, 1, 2};
  CHECK(v.has_leading_one());
}

TEST_CASE("has_leading_one returns false if leading element is 3") {
  Vector v{0, 3, 0};
  CHECK(!v.has_leading_one());
}

TEST_CASE("has_leading_one returns false for zero vector") {
  Vector v{0, 0, 0};
  CHECK(!v.has_leading_one());
}

TEST_CASE("is_standard_basis returns true if one element is 1 and rest 0") {
  Vector v{0, 1, 0};
  CHECK(v.is_standard_basis());
}

TEST_CASE("is_standard_basis returns false if two elements are 1 and rest 0") {
  Vector v{0, 1, 1};
  CHECK(!v.is_standard_basis());
}

TEST_CASE("is_standard_basis returns false if one element is 2 and rest 0") {
  Vector v{0, 2, 0};
  CHECK(!v.is_standard_basis());
}

TEST_CASE("is_standard_basis returns false for zero vector") {
  Vector v{0, 0, 0};
  CHECK(!v.is_standard_basis());
}
