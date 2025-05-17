#include <vector>
#include <stdexcept>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

class Vector
{
public:
    Vector(int s);
    // std::vector [] does no range checking
    double& operator[](int i) { return elem.at(i); }
    double operator[](int i) const { return elem.at(i); }

    int size() const { return elem.size(); }
    Vector operator+(const Vector& v) const;
    Vector operator*(int c) const;
    
private:
    std::vector<double> elem;
};


Vector::Vector(int s)
    : elem(s)    // construct the vector with s default-initialised elements
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
        result[i] = elem[i] + v[i];
    }
    return result;
}


Vector Vector::operator*(int c) const
{
    Vector result(size());
    for (int i = 0; i < size(); i++) {
        result[i] = elem[i] * c;
    }
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


TEST_CASE("Vector addition happy path")
{
    int size = 2;
    Vector u(size);
    u[0] = 1;
    u[1] = 2;
    Vector v(size);
    v[0] = 2;
    v[1] = 2;
    Vector sum = u + v;
    CHECK(sum.size() == size);
    CHECK(sum[0] == 3);
    CHECK(sum[1] == 4);
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
    Vector v(2);
    v[0] = -2;
    v[1] = 4;
    Vector cv = v * c;
    CHECK(cv[0] == -4);
    CHECK(cv[1] == 8);
}
