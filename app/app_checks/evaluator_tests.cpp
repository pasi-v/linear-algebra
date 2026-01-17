#include "doctest/doctest.h"

#include "helpers.hpp"

TEST_CASE("evaluator can define vectors and compute dot product") {
    std::unordered_map<std::string, Value> symbols;
    CHECK(run_line("vec a = [1, 2, 3]", symbols).err.empty());
    CHECK(run_line("vec b = [4, 5, 6]", symbols).err.empty());

    auto result = run_line("dot a b", symbols);
    CHECK(result.err.empty());
    CHECK(result.out == "32\n");
}

TEST_CASE("evaluator can define matrices and compute determinant") {
    std::unordered_map<std::string, Value> symbols;
    CHECK(run_line("mat M = [[1,2],[3,4]]", symbols).err.empty());

    auto result = run_line("det M", symbols);
    CHECK(result.err.empty());
    CHECK(result.out == "-2\n");
}

TEST_CASE("evaluator reports unknown symbols") {
    std::unordered_map<std::string, Value> symbols;
    auto result = run_line("dot a b", symbols);
    CHECK(result.out.empty());
    CHECK(result.err.find("unknown symbol") != std::string::npos);
}
