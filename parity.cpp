#include "doctest.h"
#include <bitset>

/**
 * Determine whether bitset has a parity error.  A parity error is to
 * have an odd number of bits.
 *
 * @param bits the bitset containing the parity bit.
 * @return true if the bitset has odd number of one bits, false
 * otherwise.
 */
template <std::size_t N> bool has_parity_error(const std::bitset<N> &bits) {
    return (bits.count() % 2) != 0;
}

TEST_CASE("1010") {
    std::bitset<4> bits("1010");
    CHECK(!has_parity_error(bits));
}

TEST_CASE("111011") {
    std::bitset<6> bits("111011");
    CHECK(has_parity_error(bits));
}

TEST_CASE("010111") {
    std::bitset<6> bits("010111");
    CHECK(!has_parity_error(bits));
}

TEST_CASE("11010111") {
    std::bitset<8> bits("11010111");
    CHECK(!has_parity_error(bits));
}
