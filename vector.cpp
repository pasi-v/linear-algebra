#include <stdexcept>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

class Vector
{
public:
    Vector(int s);
    double& operator[](int i);
    int size() { return sz; }
    Vector operator+(Vector v);
    Vector operator*(int c);
    
private:
    double*  elem;    // pointer to elements
    int sz;           // number of elements
};


Vector::Vector(int s)
{
    if (s < 0)
        throw std::length_error{"Vector size can't be negative"};
    elem = new double[s];
    sz = s;
}


double& Vector::operator[](int i)
{
    if (i < 0 || i >= size())
        throw std::out_of_range{"Vector::operator[]"};
    return elem[i];
}


Vector Vector::operator+(Vector v)
{
    if (sz != v.size())
        throw std::invalid_argument("Vector sizes must match for addition");
    Vector sum(sz);
    for (int i = 0; i < sz; i++) {
        sum[i] = elem[i] + v[i];
    }
    return sum;
}


Vector Vector::operator*(int c)
{
    Vector result(sz);
    for (int i = 0; i < sz; i++) {
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
