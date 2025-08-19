#include "doctest.h"
#include <bitset>
#include <iterator>
#include <numeric>
#include <type_traits>

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

/**
 * Calculate the parity bit for the bitset.  The parity bit will make
 * the number of bits in the bitset even.
 *
 * @param bits the bitset without the parity bit.
 * @return true if the parity bit should be 1, false if 0
 */
template <std::size_t N> bool parity_bit(const std::bitset<N> &bits) {
    return (bits.count() % 2) != 0;
}

/**
 * Calculate the check digit for v in modulo m.  The check digit
 * appended to the vector will make dot product of v and one-vector 0.
 * If m is 1, the result is always 0 regardless of the vector.
 *
 * @param v the vector without check digit
 * @param m the modulo
 * @return the check digit
 * @throws std::invalid_argument if m <= 0
 */
template <typename It, typename T = typename std::iterator_traits<It>::value_type>
T check_digit(It first, It last, T m) {
    static_assert(std::is_integral<T>::value, "check_digit requires an integral type");
    if (m <= 0) {
        throw std::invalid_argument("m must be positive");
    }

    if (m == 1)
        return 0;

    T s = std::accumulate(first, last, T{0});
    return (m - (s % m)) % m;
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

TEST_CASE("Parity bit for 1011") {
    std::bitset<4> bits("1011");
    CHECK(parity_bit(bits) == true);
}

TEST_CASE("Parity bit for 11011") {
    std::bitset<5> bits("11011");
    CHECK(parity_bit(bits) == false);
}

TEST_CASE("Check digit for [1,2,2,2] in Z3 is 2") {
    std::vector<long> v{1, 2, 2, 2};
    CHECK_EQ(2L, check_digit(v.begin(), v.end(), 3L));
}

TEST_CASE("Check digit for [3,4,2,3] in Z5 is 3") {
    std::vector<int> v{3, 4, 2, 3};
    CHECK_EQ(3, check_digit(v.begin(), v.end(), 5));
}

TEST_CASE("Check digit for [1,5,6,4,5] in Z7 is 0") {
    std::vector<int> v{1, 5, 6, 4, 5};
    CHECK_EQ(0, check_digit(v.begin(), v.end(), 7));
}

TEST_CASE("Check digit for [3,0,7,5,6,8] in Z9 is 7") {
    std::vector<int> v{3, 0, 7, 5, 6, 8};
    CHECK_EQ(7, check_digit(v.begin(), v.end(), 9));
}

TEST_CASE("Check digit with 0 modulo throws") {
    std::vector<int> v{1, 2};
    CHECK_THROWS_AS(check_digit(v.begin(), v.end(), 0), std::invalid_argument);
}

TEST_CASE("Check digit with negative modulo throws") {
    std::vector<int> v{1, 2};
    CHECK_THROWS_AS(check_digit(v.begin(), v.end(), -1), std::invalid_argument);
}

TEST_CASE("Check digit for modulo 1 is 0") {
    std::vector<int> v{1, 5, 6, 4, 5};
    CHECK_EQ(0, check_digit(v.begin(), v.end(), 1));
}

TEST_CASE("Check digit works with array type") {
    int arr[] = {3, 0, 7, 5, 6, 8};
    CHECK_EQ(7, check_digit(std::begin(arr), std::end(arr), 9));
}
