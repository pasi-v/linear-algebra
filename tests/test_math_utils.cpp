#include "doctest/doctest.h"

#include "math_utils/math_utils.hpp"

using namespace math_utils;

TEST_CASE("Radians to degrees and back") {
    CHECK(doctest::Approx(toDegrees(0)).epsilon(0.001) == 0);
    CHECK(doctest::Approx(toDegrees(M_PI)).epsilon(0.001) == 180.0);
    CHECK(doctest::Approx(toDegrees(M_PI_2)).epsilon(0.001) == 90.0);

    CHECK(doctest::Approx(toRadians(180.0)).epsilon(0.001) == M_PI);
    CHECK(doctest::Approx(toRadians(90.0)).epsilon(0.001) == M_PI_2);
}

TEST_CASE("nearly_equal") {
    CHECK(nearly_equal(1.0, 1.0 + 1e-13));
    CHECK(nearly_equal(1e8, 1e8 + 1e-4));
    CHECK(nearly_equal(0.0, 1e-13));
    CHECK(!nearly_equal(0.0, 1e-10));
}
