#ifndef _UTILS_H_
#define _UTILS_H_

#include <cstddef>
#include <stdexcept>
#include <string>

namespace utils {
inline std::size_t check_nonnegative(int n, const char *what = "size") {
    if (n < 0) {
        throw std::invalid_argument(std::string("negative ") + what);
    }
    return static_cast<std::size_t>(n);
}
} // namespace utils

#endif
