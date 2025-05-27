#include "doctest.h"
#include <cmath>
#include <initializer_list>
#include <ostream>
#include <stdexcept>
#include <vector>

/**
 * An n-dimensional vector useful for linear algebra calculations
 */
class Vector {
  public:
    /** @return a Vector from the initializer list data */
    Vector(std::initializer_list<double> data) : data_(std::move(data)) {}

    /** @return a Zero Vector with size s */
    Vector(int s);

    /** @return the element at i with range check */
    double &operator[](int i) { return data_.at(i); }

    /** @return the element at i without range check */
    double operator[](int i) const { return data_.at(i); }

    /** @return true if the vector elements are the same */
    friend bool operator==(const Vector &a, const Vector &b) { return a.data_ == b.data_; }

    /** @return the size (dimensions) of the vector */
    int size() const { return data_.size(); }

    /** @return the sum of this and the other vector */
    Vector operator+(const Vector &v) const;

    /** @return this vector multiplied by the scalar c */
    Vector operator*(double c) const;

    /** @return the difference between this and the other vector */
    Vector operator-(const Vector &v) const;

    /** @return the dot product betwee this and the other vector */
    double dot_product(const Vector &v) const;

    /** @return the length (magnitude) of this vector */
    double length() const { return std::sqrt(dot_product(*this)); }

    /** @return the magnitude of the difference of the vectors */
    double difference(const Vector &v) const;

    /** @return the angle between the vectors in radians */
    double angle(const Vector &v) const;

    /** @return the projection of v onto this vector */
    Vector proj(const Vector &v) const;

  private:
    std::vector<double> data_;
};

Vector::Vector(int s)
    : data_(s) // construct the vector with s default-initialised elements
{
    if (s < 0)
        throw std::length_error{"Vector size can't be negative"};
}

Vector Vector::operator+(const Vector &v) const {
    if (size() != v.size())
        throw std::invalid_argument("Vector sizes must match for addition");
    Vector result(size());
    for (int i = 0; i < size(); i++) {
        result[i] = data_[i] + v[i];
    }
    return result;
}

Vector Vector::operator*(double c) const {
    Vector result(size());
    for (int i = 0; i < size(); i++) {
        result[i] = data_[i] * c;
    }
    return result;
}

Vector Vector::operator-(const Vector &v) const { return *this + (v * -1); }

double Vector::dot_product(const Vector &v) const {
    if (size() != v.size())
        throw std::invalid_argument("Vector sizes must match for dot product");

    double result = 0.0;

    for (int i = 0; i < size(); i++)
        result += data_[i] * v[i];

    return result;
}

double Vector::difference(const Vector &v) const { return (*this - v).length(); }

double Vector::angle(const Vector &v) const {
    return acos(dot_product(v) / (length() * v.length()));
}

Vector Vector::proj(const Vector &v) const { return *this * (dot_product(v) / dot_product(*this)); }

std::ostream &operator<<(std::ostream &os, const Vector &v) {
    os << "{ ";
    for (int i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i != v.size() - 1)
            os << ", ";
    }
    os << " }";
    return os;
}

TEST_CASE("Vector()") {
    int size = 3;
    Vector v(size);
    CHECK(v.size() == size);
}

TEST_CASE("Construct vector with negative size throws") {
    CHECK_THROWS_AS(Vector(-1), std::length_error);
}

TEST_CASE("Construct vector of size 0 does not throw") {
    int size = 0;
    Vector v(size);
    CHECK(v.size() == size);
}

TEST_CASE("Subscript operator happy path") {
    int size = 3;
    Vector v(size);
    v[0] = 1;
    CHECK(v[0] == 1);
}

TEST_CASE("Subscript operator negative throws") {
    int size = 3;
    Vector v(size);
    CHECK_THROWS_AS(v[-1] = 1, std::out_of_range);
}

TEST_CASE("Subscript operator equal to size throws") {
    int size = 3;
    Vector v(size);
    CHECK_THROWS_AS(v[size] = 1, std::out_of_range);
}

TEST_CASE("Subscript operator larger than size throws") {
    int size = 3;
    Vector v(size);
    CHECK_THROWS_AS(v[size + 1] = 1, std::out_of_range);
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
    Vector expected = {3, 4};
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
    Vector expected = {-4, 8};
    CHECK(cv == expected);
}

TEST_CASE("Vector subtraction happy path") {
    Vector u{1, 2};
    Vector v{2, 2};
    Vector diff = u - v;
    Vector expected = {-1, 0};
    CHECK(diff == expected);
}

TEST_CASE("Vector subtraction different sizes throws") {
    Vector u(2);
    Vector v(3);
    CHECK_THROWS_AS(u + v, std::invalid_argument);
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
    Vector u(2);
    Vector v(3);
    CHECK_THROWS_AS(u.angle(v), std::invalid_argument);
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
