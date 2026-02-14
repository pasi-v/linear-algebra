#include "doctest/doctest.h"
#include "la/approx.hpp"
#include "la/vector.hpp"
#include "math_utils/math_utils.hpp"

TEST_CASE("Vector()") {
    using la::Vector;
    int size = 3;
    Vector v(size);
    CHECK(v.size() == size);
}

TEST_CASE("Construct vector of size 0 does not throw") {
    using la::Vector;
    int size = 0;
    Vector v(size);
    CHECK(v.size() == size);
}

TEST_CASE("Empty vector is empty") {
    using la::Vector;
    Vector v;
    CHECK(v.empty());
}

TEST_CASE("Non-empty vector is not empty") {
    using la::Vector;
    Vector v(1);
    CHECK(!v.empty());
}

TEST_CASE("Subscript operator happy path") {
    using la::Vector;
    int size = 3;
    Vector v(size);
    v[0] = 1;
    CHECK(v.at(0) == 1);
}

TEST_CASE("Subscript operator negative throws") {
    using la::Vector;
    int size = 3;
    Vector v(size);
    CHECK_THROWS_AS(v.at(-1) = 1, std::out_of_range);
}

TEST_CASE("Subscript operator equal to size throws") {
    using la::Vector;
    int size = 3;
    Vector v(size);
    CHECK_THROWS_AS(v.at(size) = 1, std::out_of_range);
}

TEST_CASE("Subscript operator larger than size throws") {
    using la::Vector;
    int size = 3;
    Vector v(size);
    CHECK_THROWS_AS(v.at(size + 1) = 1, std::out_of_range);
}

TEST_CASE("Vector comparison") {
    using la::Vector;
    Vector u{1.0, 2.0};
    Vector v{1.0, 2.0};
    Vector w{1.0, 3.0};

    CHECK(u == v);
    CHECK_FALSE(u == w);
}

TEST_CASE("Vector addition happy path") {
    using la::Vector;
    Vector u{1, 2};
    Vector v{2, 2};
    Vector sum = u + v;
    Vector expected{3, 4};
    CHECK(sum == expected);
}

TEST_CASE("Vector addition different sizes throws") {
    using la::Vector;
    Vector u(2);
    Vector v(3);
    CHECK_THROWS_AS(u + v, std::invalid_argument);
}

TEST_CASE("Vector subtraction happy path") {
    using la::Vector;
    Vector u{1, 2};
    Vector v{2, 2};
    Vector diff = u - v;
    Vector expected{-1, 0};
    CHECK(diff == expected);
}

TEST_CASE("Vector subtraction different sizes throws") {
    using la::Vector;
    Vector u(2);
    Vector v(3);
    CHECK_THROWS_AS(u + v, std::invalid_argument);
}

TEST_CASE("Vector::subvector basics") {
    using la::Vector;
    Vector v{10, 20, 30, 40, 50};

    SUBCASE("middle segment") {
        auto s = v.subvector(1, 3); // {20, 30, 40}
        CHECK(s.size() == 3);
        CHECK(s[0] == doctest::Approx(20));
        CHECK(s[1] == doctest::Approx(30));
        CHECK(s[2] == doctest::Approx(40));
    }

    SUBCASE("to end") {
        auto s = v.subvector(2); // {30, 40, 50}
        CHECK(s.size() == 3);
        CHECK(s[0] == doctest::Approx(30));
        CHECK(s[2] == doctest::Approx(50));
    }

    SUBCASE("empty at end") {
        auto s = v.subvector(5, 0);
        CHECK(s.size() == 0);
    }

    SUBCASE("start > size throws") {
        CHECK_THROWS_AS(v.subvector(6, 0), std::out_of_range);
    }

    SUBCASE("range exceeds size throws") {
        CHECK_THROWS_AS(v.subvector(4, 2), std::out_of_range);
    }
}

TEST_CASE("first_non_zero_column returns 0 if first element is non-zero") {
    using la::Vector;
    Vector v{1, 2, 3};
    CHECK_EQ(0, v.first_non_zero_column());
}

TEST_CASE(
    "first_non_zero_column returns m-1 if only last element is non-zero") {
    using la::Vector;
    Vector v{0, 0, 1};
    CHECK_EQ(2, v.first_non_zero_column());
}

TEST_CASE("first_non_zero_column returns -1 for zero vector") {
    using la::Vector;
    Vector v{0, 0, 0};
    CHECK_EQ(-1, v.first_non_zero_column());
}

TEST_CASE("leading_element returns first element if it is not zero") {
    using la::Vector;
    Vector v{1, 2, 3};
    CHECK_EQ(1, v.leading_element());
}

TEST_CASE("leading_element returns last element if preceding ones are zero") {
    using la::Vector;
    Vector v{0, 0, 4};
    CHECK_EQ(4, v.leading_element());
}

TEST_CASE("leading_element returns 0 for zero vector") {
    using la::Vector;
    Vector v{0, 0, 0};
    CHECK_EQ(0, v.leading_element());
}

TEST_CASE("has_leading_one returns true if leading element is 1") {
    using la::Vector;
    Vector v{0, 1, 2};
    CHECK(v.has_leading_one());
}

TEST_CASE("has_leading_one returns false if leading element is 3") {
    using la::Vector;
    Vector v{0, 3, 0};
    CHECK(!v.has_leading_one());
}

TEST_CASE("has_leading_one returns false for zero vector") {
    using la::Vector;
    Vector v{0, 0, 0};
    CHECK(!v.has_leading_one());
}

TEST_CASE("head") {
    using la::Vector;

    SUBCASE("first two elements of 3-element vector") {
        Vector v({1, 2, 3});
        CHECK_EQ(Vector({1, 2}), v.head(2));
    }

    SUBCASE("first element of a 3-element vector") {
        Vector v({1, 2, 3});
        CHECK_EQ(Vector({1}), v.head(1));
    }

    SUBCASE("all elements of a 3-element vector") {
        Vector v({1, 2, 3});
        CHECK_EQ(Vector({1, 2, 3}), v.head(3));
    }

    SUBCASE("n > len elements of a 3-element vector") {
        Vector v({1, 2, 3});
        CHECK_EQ(Vector({1, 2, 3}), v.head(4));
    }

    SUBCASE("zero elements of a 3-element vector") {
        Vector v({1, 2, 3});
        CHECK_EQ(Vector({}), v.head(0));
    }
}

TEST_CASE("tail") {
    using la::Vector;

    SUBCASE("tail of 3-element vector") {
        Vector v({1, 2, 3});
        CHECK_EQ(Vector({2, 3}), v.tail());
    }

    SUBCASE("start = 2 tail of 3-element vector") {
        Vector v({1, 2, 3});
        CHECK_EQ(Vector({3}), v.tail(2));
    }

    SUBCASE("tail of 1-element vector is empty") {
        Vector v({1});
        CHECK_EQ(Vector(0), v.tail());
    }

    SUBCASE("start > size returns empty vector") {
        Vector v({1, 2, 3});
        CHECK_EQ(Vector(0), v.tail(3));
    }
}

TEST_CASE("approx_equal") {
    using la::Vector;

    SUBCASE("Vector is approximately equal to itself") {
        Vector v({1, 2, 3});
        CHECK(approx_equal(v, v, math_utils::kDefaultAbsTol,
                           math_utils::kDefaultRelTol));
    }

    SUBCASE("Vector is approximately equal to a vector with same values") {
        Vector u({1, 2, 3});
        Vector v({1, 2, 3});
        CHECK(approx_equal(u, v, math_utils::kDefaultAbsTol,
                           math_utils::kDefaultRelTol));
    }

    SUBCASE(
        "Vectors are approximately equal with values approximately equal") {
        Vector u({1 + 1e-14, 2 - 1e-14});
        Vector v({1, 2});
        CHECK(approx_equal(u, v, math_utils::kDefaultAbsTol,
                           math_utils::kDefaultRelTol));
    }

    SUBCASE("Not approximately equal if not close enough") {
        Vector u({1 + 1e-5, 2 - 1e-5});
        Vector v({1, 2});
        CHECK(!approx_equal(u, v, math_utils::kDefaultAbsTol,
                            math_utils::kDefaultRelTol));
    }
}
