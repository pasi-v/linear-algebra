#ifndef VECTOR_ALGORITHMS_HPP
#define VECTOR_ALGORITHMS_HPP

#include "vector.hpp"

namespace la {
/**
 * @return the dot product between the vectors
 * @throws std::invalid_argument if the vector sizes don't match
 */
double dot(const Vector &, const Vector &);

/** @return the norm (length, magnitude) of the vector */
double norm(const Vector &);

/**
 * @return the angle between the vectors in radians
 * @throws std::invalid_argument if the vector sizes don't match
 * @throws std::domain_error if either vector is zero length within eps
 */
double angle(const Vector &u, const Vector &v, double eps = 1e-12);

/**
 * @return the projection of v onto this vector
 * @throws std::invalid_argument if the vector sizes don't match
 */
Vector proj_onto(const Vector &onto, const Vector &v);

/**
 * @return the distance (magnitude of the difference) of the vectors
 * @throws std::invalid_argument if the vector sizes don't match
 */
double distance(const Vector &u, const Vector &v);

/** @return true if all elements are zero, false otherwise */
bool is_zero(const Vector &);

} // namespace la

#endif // VECTOR_ALGORITHMS_HPP
