#ifndef APPROX_HPP
#define APPROX_HPP

#include "matrix.hpp"
#include "vector.hpp"

namespace la {
bool approx_equal(const Vector &a, const Vector &b, double abs_tol,
                  double rel_tol);
bool approx_equal(const Matrix &A, const Matrix &B, double abs_tol,
                  double rel_tol);
} // namespace la

#endif // APPROX_HPP
