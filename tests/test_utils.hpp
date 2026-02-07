#ifndef TEST_UTILS_HPP
#define TEST_UTILS_HPP

#include "la/approx.hpp"

constexpr double kTestAbsTol = 1e-12;
constexpr double kTestRelTol = 1e-10;

#define CHECK_NEAR(a, b) \
CHECK(la::approx_equal((a), (b), kTestAbsTol, kTestRelTol))

#endif // TEST_UTILS_HPP
