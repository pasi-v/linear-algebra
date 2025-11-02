#include "la/parity.hpp"
#include <bitset>
#include <iterator>
#include <numeric>
#include <type_traits>

namespace la {
namespace {
int upc_weighted_sum(const std::vector<int> &digits) {
    int s = 0;
    for (size_t i = 0; i < digits.size(); ++i)
        s += (i % 2 == 0 ? 3 : 1) * digits[i];
    return s;
}

int isbn10_weighted_sum(const std::vector<int> &digits) {
    int s = 0;
    for (size_t i = 0; i < digits.size(); ++i)
        s += ((10 - i) * digits[i]);
    return s;
}

} // namespace

int upc_check_digit(std::vector<int> v) {
    if (v.size() != 11) {
        throw std::invalid_argument("UPC digits vector must be length 11");
    }
    int s = upc_weighted_sum(v);
    return modular_additive_inverse(s, 10);
}

bool upc_has_error(std::vector<int> v) {
    if (v.size() != 12) {
        throw std::invalid_argument("UPC digits vector must be length 12");
    }
    int s = upc_weighted_sum(v);
    return !(s % 10 == 0);
}

int isbn10_check_digit(std::vector<int> v) {
    if (v.size() != 9) {
        throw std::invalid_argument("ISBN digits vector must be length 9");
    }
    int s = isbn10_weighted_sum(v);
    return modular_additive_inverse(s, 11);
}

bool isbn10_has_error(std::vector<int> v) {
    if (v.size() != 10) {
        throw std::invalid_argument("ISBN digits vector must be length 10");
    }
    int s = isbn10_weighted_sum(v);
    return !(s % 11 == 0);
}
} // namespace la
