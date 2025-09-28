#include "doctest.h"
#include "vector3d.h"

TEST_CASE("Operator -") {
  Vector3D u = {2, 0, 2};
  Vector3D v = {0, -1, 1};
  Vector3D expected = {2, 1, 1};
  CHECK_EQ(u - v, expected);
}

TEST_CASE("Cross product") {
  Vector3D u = {0, 1, 1};
  Vector3D v = {3, -1, 2};
  Vector3D expected = {3, 3, -3};
  Vector3D n = u.crossProduct(v);
  CHECK_EQ(n, expected);
}

TEST_CASE("Dot product") {
  Vector3D u = {1, 2, -3};
  Vector3D v = {-3, 5, 2};
  double expected = 1.0;

  CHECK_EQ(u.dotProduct(v), expected);
}

TEST_CASE("Cross product and dot product is zero") {
  Vector3D u = {0, 1, 1};
  Vector3D v = {3, -1, 2};
  Vector3D uxv = u.crossProduct(v);

  CHECK_EQ(uxv.dotProduct(u), 0);
  CHECK_EQ(uxv.dotProduct(v), 0);
}
