#include "doctest/doctest.h"

#include "la/math_utils.hpp"

TEST_CASE("Radians to degrees and back") {
    using namespace math_utils;

    CHECK(doctest::Approx(toDegrees(0)).epsilon(0.001) == 0);
    CHECK(doctest::Approx(toDegrees(M_PI)).epsilon(0.001) == 180.0);
    CHECK(doctest::Approx(toDegrees(M_PI_2)).epsilon(0.001) == 90.0);

    CHECK(doctest::Approx(toRadians(180.0)).epsilon(0.001) == M_PI);
    CHECK(doctest::Approx(toRadians(90.0)).epsilon(0.001) == M_PI_2);
}
