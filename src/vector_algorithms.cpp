#include "la/vector_algorithms.hpp"
#include "la/vector.hpp"
#include <cmath>

namespace la {
double dot(const Vector &u, const Vector &v) {
    if (u.size() != v.size())
        throw std::invalid_argument("Vector sizes must match for dot product");

    double result = 0.0;

    for (std::size_t i = 0; i < u.size(); i++)
        result += u[i] * v[i];

    return result;
}

double norm(const Vector &v) { return std::sqrt(dot(v, v)); }

double angle(const Vector &u, const Vector &v, double eps) {
    // Simple implementation is:
    // return acos(dot_product(v) / (length() * v.length()));
    // But acos needs to be clamped to [-1, 1] without rounding errors drifting
    // it out
    const double asq = dot(u, u);
    const double bsq = dot(v, v);

    // Use squared tolerance to avoid an extra sqrt.
    const double tol2 = eps * eps;
    if (asq <= tol2 || bsq <= tol2) {
        throw std::domain_error("angle: undefined for zero-length vector");
    }

    const double denom = std::sqrt(asq * bsq);
    double cos_theta = dot(u, v) / denom;

    // Clamp into [-1, 1] (C++11, no std::clamp)
    if (cos_theta > 1.0)
        cos_theta = 1.0;
    else if (cos_theta < -1.0)
        cos_theta = -1.0;

    return std::acos(cos_theta);
}

Vector proj_onto(const Vector &onto, const Vector &v) {
    return onto * (dot(onto, v) / dot(onto, onto));
}

double distance(const Vector &u, const Vector &v) { return norm(u - v); }

bool is_zero(const Vector &v) {
    for (std::size_t i = 0; i < v.size(); i++) {
        if (v[i] != 0) {
            return false;
        }
    }
    return true;
}

} // namespace la
