#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

class Vector
{
public:
    Vector(int s)
        : elem { new double[s]},
          sz {s}
    {}
    int size() { return sz; }
    
private:
    double*  elem;    // pointer to elements
    int sz;           // number of elements
};


TEST_CASE("Vector()")
{
    int size = 3;
    Vector v(size);
    CHECK(v.size() == size);
}
