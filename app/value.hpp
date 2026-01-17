#ifndef LA_CALC_VALUE_HPP
#define LA_CALC_VALUE_HPP

#include "la/matrix.hpp"
#include "la/vector.hpp"

struct Value {
    enum class Kind { Vector, Matrix };
    Kind kind;
    la::Vector vec;
    la::Matrix mat;
};

#endif
