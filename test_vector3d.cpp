#include "doctest.h"
#include "vector3d.h"

TEST_CASE("Cross product") {
    Vector3D u = {0, 1, 1};
    Vector3D v = {3, -1, 2};
    Vector3D expected = {3, 3, -3};
    Vector3D n = u.crossProduct(v);
    CHECK_EQ(n, expected);
}
