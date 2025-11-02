#ifndef LA_PARITY_HPP
#define LA_PARITY_HPP

#include <bitset>
#include <numeric>

namespace la {
/**
 * Computes the modular additive inverse of a number.
 *
 * The modular additive inverse of `sum` modulo `mod` is the value `x`
 * such that `(sum + x) % mod == 0`. In other words, it returns the value
 * that, when added to `sum`, results in a multiple of `mod`.
 *
 * This implementation ensures the result is always in the range [0, mod).
 *
 * @param sum The integer whose modular additive inverse is to be found.
 * @param mod The modulus (must be a positive integer).
 * @return The modular additive inverse of `sum` modulo `mod`,
 *         in the range [0, mod).
 *
 * @example
 * modular_additive_inverse(3, 5)  // returns 2, because (3 + 2) % 5 == 0
 * modular_additive_inverse(5, 5)  // returns 0, because (5 + 0) % 5 == 0
 * modular_additive_inverse(-1, 5) // returns 1, because (-1 + 1) % 5 == 0
 */
constexpr int modular_additive_inverse(int sum, int mod) noexcept {
    return (mod - (sum % mod)) % mod;
}

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
int upc_check_digit(std::vector<int> v);

/**
 * @brief Checks whether the UPC code has an error.
 * @param v the UPC vector with check digit, each element in range [0, 9].
 * If the digits are not within the range, the check digit will be meaningless.
 * @return true if the code has an error, false if not.
 * @throws std::invalid argument if size of v != 12
 */
bool upc_has_error(std::vector<int> v);

/**
 * @brief Computes the check digit for ISBN code vector.
 * @param v the ISBN vector without check digit, each element in range [0, 9].
 * If the digits are not within the range, the check digit will be meaningless.
 * @return the check digit in the range [0, 9]
 * @throws std::invalid argument if size of v != 9
 */
int isbn10_check_digit(std::vector<int> v);

/**
 * @brief Checks whether the ISBN code has an error.
 * @param v the ISBN vector with check digit, each element in range [0, 9].
 * If the digits are not within the range, the check digit will be meaningless.
 * @return true if the code has an error, false if not.
 * @throws std::invalid argument if size of v != 10
 */
bool isbn10_has_error(std::vector<int> v);
} // namespace la

#endif
