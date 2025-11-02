#include "doctest/doctest.h"
#include "la/parity.hpp"
#include <bitset>

TEST_CASE("1010") {
    using la::has_parity_error;
    std::bitset<4> bits("1010");
    CHECK(!has_parity_error(bits));
}

TEST_CASE("111011") {
    using la::has_parity_error;
    std::bitset<6> bits("111011");
    CHECK(has_parity_error(bits));
}

TEST_CASE("010111") {
    using la::has_parity_error;
    std::bitset<6> bits("010111");
    CHECK(!has_parity_error(bits));
}

TEST_CASE("11010111") {
    using la::has_parity_error;
    std::bitset<8> bits("11010111");
    CHECK(!has_parity_error(bits));
}

TEST_CASE("Parity bit for 1011") {
    using la::parity_bit;
    std::bitset<4> bits("1011");
    CHECK(parity_bit(bits) == true);
}

TEST_CASE("Parity bit for 11011") {
    using la::parity_bit;
    std::bitset<5> bits("11011");
    CHECK(parity_bit(bits) == false);
}

TEST_CASE("Check digit for [1,2,2,2] in Z3 is 2") {
    using la::check_digit;
    std::vector<long> v{1, 2, 2, 2};
    CHECK_EQ(2L, check_digit(v.begin(), v.end(), 3L));
}

TEST_CASE("Check digit for [3,4,2,3] in Z5 is 3") {
    using la::check_digit;
    std::vector<int> v{3, 4, 2, 3};
    CHECK_EQ(3, check_digit(v.begin(), v.end(), 5));
}

TEST_CASE("Check digit for [1,5,6,4,5] in Z7 is 0") {
    using la::check_digit;
    std::vector<int> v{1, 5, 6, 4, 5};
    CHECK_EQ(0, check_digit(v.begin(), v.end(), 7));
}

TEST_CASE("Check digit for [3,0,7,5,6,8] in Z9 is 7") {
    using la::check_digit;
    std::vector<int> v{3, 0, 7, 5, 6, 8};
    CHECK_EQ(7, check_digit(v.begin(), v.end(), 9));
}

TEST_CASE("Check digit with 0 modulo throws") {
    using la::check_digit;
    std::vector<int> v{1, 2};
    CHECK_THROWS_AS(check_digit(v.begin(), v.end(), 0), std::invalid_argument);
}

TEST_CASE("Check digit with negative modulo throws") {
    using la::check_digit;
    std::vector<int> v{1, 2};
    CHECK_THROWS_AS(check_digit(v.begin(), v.end(), -1),
                    std::invalid_argument);
}

TEST_CASE("Check digit for modulo 1 is 0") {
    using la::check_digit;
    std::vector<int> v{1, 5, 6, 4, 5};
    CHECK_EQ(0, check_digit(v.begin(), v.end(), 1));
}

TEST_CASE("Check digit works with array type") {
    using la::check_digit;
    int arr[] = {3, 0, 7, 5, 6, 8};
    CHECK_EQ(7, check_digit(std::begin(arr), std::end(arr), 9));
}

TEST_CASE("UPC check digit happy path") {
    using la::upc_check_digit;
    std::vector<int> v{0, 5, 9, 4, 6, 4, 7, 0, 0, 2, 7};
    CHECK_EQ(8, upc_check_digit(v));
}

TEST_CASE("UPC check digit 0") {
    using la::upc_check_digit;
    std::vector<int> v{0, 1, 4, 0, 1, 4, 1, 8, 4, 1, 2};
    CHECK_EQ(0, upc_check_digit(v));
}

TEST_CASE("UPC check digit too few digits throws") {
    using la::upc_check_digit;
    std::vector<int> v{0, 5, 9, 4, 6, 4, 7, 0, 0, 2};
    CHECK_THROWS_AS(upc_check_digit(v), std::invalid_argument);
}

TEST_CASE("UPC check digit too many digits throws") {
    using la::upc_check_digit;
    std::vector<int> v{0, 5, 9, 4, 6, 4, 7, 0, 0, 2, 1, 1};
    CHECK_THROWS_AS(upc_check_digit(v), std::invalid_argument);
}

TEST_CASE("upc_has_error without error") {
    using la::upc_has_error;
    std::vector<int> v{0, 5, 9, 4, 6, 4, 7, 0, 0, 2, 7, 8};
    CHECK(!upc_has_error(v));
}

TEST_CASE("upc_has_error without error with 0 check digit") {
    using la::upc_has_error;
    std::vector<int> v{0, 1, 4, 0, 1, 4, 1, 8, 4, 1, 2, 0};
    CHECK(!upc_has_error(v));
}

TEST_CASE("upc_has_error with error") {
    using la::upc_has_error;
    std::vector<int> v{0, 5, 9, 4, 6, 4,
                       7, 0, 0, 2, 7, 2}; // check should be 8
    CHECK(upc_has_error(v));
}

TEST_CASE("upc_has_error too few digits throws") {
    using la::upc_has_error;
    std::vector<int> v{0, 5, 9, 4, 6, 4, 7, 0, 0, 2, 8};
    CHECK_THROWS_AS(upc_has_error(v), std::invalid_argument);
}

TEST_CASE("upc_has_error too many digits throws") {
    using la::upc_has_error;
    std::vector<int> v{0, 5, 9, 4, 6, 4, 7, 0, 0, 2, 1, 1, 8};
    CHECK_THROWS_AS(upc_has_error(v), std::invalid_argument);
}

TEST_CASE("ISBN check digit happy path") {
    using la::isbn10_check_digit;
    std::vector<int> v{0, 3, 9, 4, 7, 5, 6, 8, 2};
    CHECK_EQ(7, isbn10_check_digit(v));
}

TEST_CASE("ISBN check digit happy path, 10 as valid result") {
    using la::isbn10_check_digit;
    std::vector<int> v{0, 3, 8, 7, 9, 7, 9, 9, 3};
    CHECK_EQ(10, isbn10_check_digit(v));
}

TEST_CASE("ISBN check digit too few digits throws") {
    using la::isbn10_check_digit;
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8}; // should be 9
    CHECK_THROWS_AS(isbn10_check_digit(v), std::invalid_argument);
}

TEST_CASE("ISBN check digit too many digits throws") {
    using la::isbn10_check_digit;
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // should be 9
    CHECK_THROWS_AS(isbn10_check_digit(v), std::invalid_argument);
}

TEST_CASE("isbn10_has_error without error, with 10 as check digit") {
    using la::isbn10_has_error;
    std::vector<int> v{0, 3, 8, 7, 9, 7, 9, 9, 3, 10};
    CHECK(!isbn10_has_error(v));
}

TEST_CASE("isbn10_has_error without error, with 7 as check digit") {
    using la::isbn10_has_error;
    std::vector<int> v{0, 3, 9, 4, 7, 5, 6, 8, 2, 7};
    CHECK(!isbn10_has_error(v));
}

TEST_CASE("isbn10_has_error with error") {
    using la::isbn10_has_error;
    std::vector<int> v{0, 4, 4, 9, 5, 0, 8, 3, 5, 6};
    CHECK(isbn10_has_error(v));
}

TEST_CASE("isbn10_has_error too few digits throws") {
    using la::isbn10_has_error;
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9}; // should be 10
    CHECK_THROWS_AS(isbn10_has_error(v), std::invalid_argument);
}

TEST_CASE("isbn10_has_error too many digits throws") {
    using la::isbn10_has_error;
    std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}; // should be 10
    CHECK_THROWS_AS(isbn10_has_error(v), std::invalid_argument);
}
