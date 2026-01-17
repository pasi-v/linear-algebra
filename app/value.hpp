#ifndef LA_CALC_VALUE_HPP
#define LA_CALC_VALUE_HPP

#include "la/matrix.hpp"
#include "la/vector.hpp"

/**
 * @brief Typed value stored in the REPL symbol table.
 */
struct Value {
    /** @brief Indicates which payload is valid. */
    enum class Kind { Vector, Matrix };
    /** @brief Discriminant describing which payload is active. */
    Kind kind;
    /** @brief Vector payload when kind == Kind::Vector. */
    la::Vector vec;
    /** @brief Matrix payload when kind == Kind::Matrix. */
    la::Matrix mat;
};

#endif
