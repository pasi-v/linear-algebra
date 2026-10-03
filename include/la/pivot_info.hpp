#ifndef PIVOT_INFO_HPP
#define PIVOT_INFO_HPP

#include "matrix.hpp"
#include <cstddef>
#include <vector>

namespace la {
struct PivotInfo {
    std::vector<std::size_t> pivot_cols;
    std::vector<std::size_t> free_cols;
};
} // namespace la

#endif // PIVOT_INFO_HPP
