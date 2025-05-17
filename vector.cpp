#include <cmath>
#include <initializer_list>
#include <vector>
#include <stdexcept>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

class Vector
{
public:
    Vector(std::initializer_list<double> data) : data_(std::move(data)) {}
    Vector(int s);
    // std::vector [] does no range checking
    double& operator[](int i) { return data_.at(i); }
    double operator[](int i) const { return data_.at(i); }

    friend bool operator==(const Vector& a, const Vector& b) {
        return a.data_ == b.data_;
    }

    int size() const { return data_.size(); }
    Vector operator+(const Vector& v) const;
    Vector operator*(int c) const;
    Vector operator-(const Vector& v) const;

    double dot_product(const Vector& v) const;
    double length() const { return std::sqrt(dot_product(*this)); }
    
private:
    std::vector<double> data_;
};


Vector::Vector(int s)
    : data_(s)    // construct the vector with s default-initialised elements
{
    if (s < 0)
        throw std::length_error{"Vector size can't be negative"};
}


Vector Vector::operator+(const Vector& v) const
{
    if (size() != v.size())
        throw std::invalid_argument("Vector sizes must match for addition");
    Vector result(size());
    for (int i = 0; i < size(); i++) {
        result[i] = data_[i] + v[i];
    }
    return result;
}


Vector Vector::operator*(int c) const
{
    Vector result(size());
    for (int i = 0; i < size(); i++) {
        result[i] = data_[i] * c;
    }
    return result;
}


Vector Vector::operator-(const Vector &v) const { return *this + (v * -1); }


double Vector::dot_product(const Vector& v) const
{
    if (size() != v.size())
        throw std::invalid_argument("Vector sizes must match for dot product");

    double result = 0.0;

    for (int i = 0; i < size(); i++)
        result += data_[i] * v[i];

    return result;
}


TEST_CASE("Vector()")
{
    int size = 3;
    Vector v(size);
    CHECK(v.size() == size);
}


TEST_CASE("Construct vector with negative size throws")
{
    CHECK_THROWS_AS(Vector(-1), std::length_error);
}


TEST_CASE("Construct vector of size 0 does not throw")
{
    int size = 0;
    Vector v(size);
    CHECK(v.size() == size);
}


TEST_CASE("Subscript operator happy path")
{
    int size = 3;
    Vector v(size);
    v[0] = 1;
    CHECK(v[0] == 1);
}


TEST_CASE("Subscript operator negative throws")
{
    int size = 3;
    Vector v(size);
    CHECK_THROWS_AS(v[-1] = 1, std::out_of_range);
}


TEST_CASE("Subscript operator equal to size throws")
{
    int size = 3;
    Vector v(size);
    CHECK_THROWS_AS(v[size] = 1, std::out_of_range);
}


TEST_CASE("Subscript operator larger than size throws")
{
    int size = 3;
    Vector v(size);
    CHECK_THROWS_AS(v[size + 1] = 1, std::out_of_range);
}


TEST_CASE("Vector comparison")
{
    Vector u{1.0, 2.0};
    Vector v{1.0, 2.0};
    Vector w{1.0, 3.0};

    CHECK(u == v);
    CHECK_FALSE(u == w);
}


TEST_CASE("Vector addition happy path")
{
    Vector u{1, 2};
    Vector v{2, 2};
    Vector sum = u + v;
    Vector expected = {3, 4};
    CHECK(sum == expected);
}


TEST_CASE("Vector addition different sizes throws")
{
    Vector u(2);
    Vector v(3);
    CHECK_THROWS_AS(u + v, std::invalid_argument);
}


TEST_CASE("Vector scalar multiplication happy path")
{
    int c = 2;
    Vector v{-2, 4};
    Vector cv = v * c;
    Vector expected = {-4, 8};
    CHECK(cv == expected);
}


TEST_CASE("Vector subtraction happy path")
{
    Vector u{1, 2};
    Vector v{2, 2};
    Vector diff = u - v;
    Vector expected = {-1, 0};
    CHECK(diff == expected);
}


TEST_CASE("Vector subtraction different sizes throws")
{
    Vector u(2);
    Vector v(3);
    CHECK_THROWS_AS(u + v, std::invalid_argument);
}


TEST_CASE("Dot product happy case")
{
    Vector u{1, 2, -3};
    Vector v{-3, 5, 2};
    double result = u.dot_product(v);
    CHECK(result == 1.0);
}


TEST_CASE("Dot product different sizes throws")
{
    Vector u(2);
    Vector v(3);
    CHECK_THROWS_AS(u.dot_product(v), std::invalid_argument);
}


TEST_CASE("Vector length")
{
    Vector u{2, 3};
    CHECK(u.length() == std::sqrt(13.0));
}
