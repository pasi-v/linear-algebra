#ifndef LA_VECTOR_ALGORITHMS_HPP
#define LA_VECTOR_ALGORITHMS_HPP

#include "math_utils/math_utils.hpp"
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
 * @return the projection of v onto vector 'onto'
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

/**
 * @return true if this is a standard basis vector, i.e. has 1 in one
 * element and the rest are zeros. */
bool is_standard_basis(const Vector &);

/** @return index of first non-zero element, -1 if all zeroes */
int first_non_zero_column(const Vector &);

/** @return first non-zero element, 0 if all zeros */
double leading_element(const Vector &);

/** @return true if leading element is 1, false otherwise */
inline bool has_leading_one(const Vector &v) {
    return math_utils::nearly_equal(leading_element(v), 1.0);
}

/**
 *  @brief Determine whether b lies in the span of the given vectors
 *  @param vectors
 *  @param b
 *  @return true if there exist scalars c1...cn such that c1v1 + ... + cnvn = b
 *  @throws std::invalid_argument if the vector sizes (including b) don't match
 */
bool is_in_span(std::vector<Vector> &vectors, Vector &b);

} // namespace la

#endif // LA_VECTOR_ALGORITHMS_HPP
