#include "doctest/doctest.h"
#include <bitset>
#include <iterator>
#include <numeric>
#include <type_traits>

// This unnamed namespace is internal to the compilation unit, i.e. this file.
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

int modular_additive_inverse(int sum, int mod) {
    return (mod - (sum % mod)) % mod;
}

} // namespace

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
 * @brief Computes the check digit that makes the sum ≡ 0 (mod m).
 * @tparam It Input iterator type whose value_type must be an integral type.
 * @tparam T Integral type used for accumulation and return value.
 * @param first Beginning of the input range.
 * @param last End of the input range.
 * @param m the modulus (m > 0)
 * @return the check digit in the range [0, m-1]
 * @throws std::invalid_argument if m <= 0
 */
template <typename It,
          typename T = typename std::iterator_traits<It>::value_type>
T check_digit(It first, It last, T m) {
    static_assert(std::is_integral<T>::value,
                  "check_digit requires an integral type");
    if (m <= 0) {
        throw std::invalid_argument("m must be positive");
    }

    if (m == 1)
        return 0;

    T s = std::accumulate(first, last, T{0});
    return modular_additive_inverse(s, m);
}

/**
 * @brief Computes the check digit for UPC code vector.
 * @param v the UPC vector without check digit, each element in range [0, 9].
 * If the digits are not within the range, the check digit will be meaningless.
 * @return the check digit in the range [0, 9]
 * @throws std::invalid argument if size of v != 11
 */
int upc_check_digit(std::vector<int> v) {
    if (v.size() != 11) {
        throw std::invalid_argument("UPC digits vector must be length 11");
    }
    int s = upc_weighted_sum(v);
    return modular_additive_inverse(s, 10);
}

/**
 * @brief Checks whether the UPC code has an error.
 * @param v the UPC vector with check digit, each element in range [0, 9].
 * If the digits are not within the range, the check digit will be meaningless.
 * @return true if the code has an error, false if not.
 * @throws std::invalid argument if size of v != 12
 */
bool upc_has_error(std::vector<int> v) {
    if (v.size() != 12) {
        throw std::invalid_argument("UPC digits vector must be length 12");
    }
    int s = upc_weighted_sum(v);
    return !(s % 10 == 0);
}

/**
 * @brief Computes the check digit for ISBN code vector.
 * @param v the ISBN vector without check digit, each element in range [0, 9].
 * If the digits are not within the range, the check digit will be meaningless.
 * @return the check digit in the range [0, 9]
 * @throws std::invalid argument if size of v != 9
 */
int isbn10_check_digit(std::vector<int> v) {
    if (v.size() != 9) {
        throw std::invalid_argument("ISBN digits vector must be length 9");
    }
    int s = isbn10_weighted_sum(v);
    return modular_additive_inverse(s, 11);
}

/**
 * @brief Checks whether the ISBN code has an error.
 * @param v the ISBN vector with check digit, each element in range [0, 9].
 * If the digits are not within the range, the check digit will be meaningless.
 * @return true if the code has an error, false if not.
 * @throws std::invalid argument if size of v != 10
 */
bool isbn10_has_error(std::vector<int> v) {
    if (v.size() != 10) {
        throw std::invalid_argument("ISBN digits vector must be length 10");
    }
    int s = isbn10_weighted_sum(v);
    return !(s % 11 == 0);
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
    CHECK_THROWS_AS(check_digit(v.begin(), v.end(), -1),
                    std::invalid_argument);
}

TEST_CASE("Check digit for modulo 1 is 0") {
    std::vector<int> v{1, 5, 6, 4, 5};
    CHECK_EQ(0, check_digit(v.begin(), v.end(), 1));
}

TEST_CASE("Check digit works with array type") {
    int arr[] = {3, 0, 7, 5, 6, 8};
    CHECK_EQ(7, check_digit(std::begin(arr), std::end(arr), 9));
}

TEST_CASE("UPC check digit happy path") {
    std::vector<int> v{0, 5, 9, 4, 6, 4, 7, 0, 0, 2, 7};
    CHECK_EQ(8, upc_check_digit(v));
}

TEST_CASE("UPC check digit 0") {
    std::vector<int> v{0, 1, 4, 0, 1, 4, 1, 8, 4, 1, 2};
    CHECK_EQ(0, upc_check_digit(v));
}

TEST_CASE("UPC check digit too few digits throws") {
    std::vector<int> v{0, 5, 9, 4, 6, 4, 7, 0, 0, 2};
    CHECK_THROWS_AS(upc_check_digit(v), std::invalid_argument);
}

TEST_CASE("UPC check digit too many digits throws") {
    std::vector<int> v{0, 5, 9, 4, 6, 4, 7, 0, 0, 2, 1, 1};
    CHECK_THROWS_AS(upc_check_digit(v), std::invalid_argument);
}

TEST_CASE("upc_has_error without error") {
    std::vector<int> v{0, 5, 9, 4, 6, 4, 7, 0, 0, 2, 7, 8};
    CHECK(!upc_has_error(v));
}

TEST_CASE("upc_has_error without error with 0 check digit") {
    std::vector<int> v{0, 1, 4, 0, 1, 4, 1, 8, 4, 1, 2, 0};
    CHECK(!upc_has_error(v));
}

TEST_CASE("upc_has_error with error") {
    std::vector<int> v{0, 5, 9, 4, 6, 4,
                       7, 0, 0, 2, 7, 2}; // check should be 8
    CHECK(upc_has_error(v));
}

TEST_CASE("upc_has_error too few digits throws") {
    std::vector<int> v{0, 5, 9, 4, 6, 4, 7, 0, 0, 2, 8};
    CHECK_THROWS_AS(upc_has_error(v), std::invalid_argument);
}

TEST_CASE("upc_has_error too many digits throws") {
    std::vector<int> v{0, 5, 9, 4, 6, 4, 7, 0, 0, 2, 1, 1, 8};
    CHECK_THROWS_AS(upc_has_error(v), std::invalid_argument);
}

TEST_CASE("ISBN check digit happy path") {
    std::vector<int> v{0, 3, 9, 4, 7, 5, 6, 8, 2};
    CHECK_EQ(7, isbn10_check_digit(v));
}

TEST_CASE("ISBN check digit happy path, 10 as valid result") {
    std::vector<int> v{0, 3, 8, 7, 9, 7, 9, 9, 3};
    CHECK_EQ(10, isbn10_check_digit(v));
}

TEST_CASE("ISBN check digit too few digits throws") {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8}; // should be 9
    CHECK_THROWS_AS(isbn10_check_digit(v), std::invalid_argument);
}

TEST_CASE("ISBN check digit too many digits throws") {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // should be 9
    CHECK_THROWS_AS(isbn10_check_digit(v), std::invalid_argument);
}

TEST_CASE("isbn10_has_error without error, with 10 as check digit") {
    std::vector<int> v{0, 3, 8, 7, 9, 7, 9, 9, 3, 10};
    CHECK(!isbn10_has_error(v));
}

TEST_CASE("isbn10_has_error without error, with 7 as check digit") {
    std::vector<int> v{0, 3, 9, 4, 7, 5, 6, 8, 2, 7};
    CHECK(!isbn10_has_error(v));
}

TEST_CASE("isbn10_has_error with error") {
    std::vector<int> v{0, 4, 4, 9, 5, 0, 8, 3, 5, 6};
    CHECK(isbn10_has_error(v));
}

TEST_CASE("isbn10_has_error too few digits throws") {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9}; // should be 10
    CHECK_THROWS_AS(isbn10_has_error(v), std::invalid_argument);
}

TEST_CASE("isbn10_has_error too many digits throws") {
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}; // should be 10
    CHECK_THROWS_AS(isbn10_has_error(v), std::invalid_argument);
}
