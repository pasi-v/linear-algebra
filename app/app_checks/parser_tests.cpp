#include "doctest/doctest.h"

#include "parser.hpp"

TEST_CASE("parse_identifier consumes valid tokens") {
    Parser p("alpha_1 rest");
    CHECK(p.parse_identifier() == "alpha_1");
}

TEST_CASE("parse_number handles integers and floats") {
    Parser p("3.14 2");
    CHECK(p.parse_number() == doctest::Approx(3.14));
    CHECK(p.parse_number() == doctest::Approx(2.0));
}

TEST_CASE("parse_vector_literal parses bracketed lists") {
    Parser p("[1, 2, 3]");
    auto v = p.parse_vector_literal();
    REQUIRE(v.size() == 3);
    CHECK(v[0] == doctest::Approx(1.0));
    CHECK(v[1] == doctest::Approx(2.0));
    CHECK(v[2] == doctest::Approx(3.0));
}

TEST_CASE("parse_matrix_literal parses rows") {
    Parser p("[[1,2],[3,4]]");
    auto m = p.parse_matrix_literal();
    REQUIRE(m.size() == 2);
    REQUIRE(m[0].size() == 2);
    REQUIRE(m[1].size() == 2);
    CHECK(m[0][0] == doctest::Approx(1.0));
    CHECK(m[1][1] == doctest::Approx(4.0));
}
