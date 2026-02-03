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

TEST_CASE("evaluator can compute rref of a matrix") {
    std::unordered_map<std::string, Value> symbols;
    CHECK(run_line("mat M = [[2, 1, -1], [-3, -1, 2], [-2, 1, 2]]", symbols).err.empty());

    auto result = run_line("rref M", symbols);
    CHECK(result.err.empty());
    // Should produce identity matrix
    CHECK(result.out.find("[ 1, 0, 0 ]") != std::string::npos);
    CHECK(result.out.find("[ 0, 1, 0 ]") != std::string::npos);
    CHECK(result.out.find("[ 0, 0, 1 ]") != std::string::npos);
}

TEST_CASE("evaluator rref preserves identity matrix") {
    std::unordered_map<std::string, Value> symbols;
    CHECK(run_line("mat I = [[1, 0, 0], [0, 1, 0], [0, 0, 1]]", symbols).err.empty());

    auto result = run_line("rref I", symbols);
    CHECK(result.err.empty());
    CHECK(result.out.find("[ 1, 0, 0 ]") != std::string::npos);
    CHECK(result.out.find("[ 0, 1, 0 ]") != std::string::npos);
    CHECK(result.out.find("[ 0, 0, 1 ]") != std::string::npos);
}

TEST_CASE("evaluator rref handles singular matrix") {
    std::unordered_map<std::string, Value> symbols;
    CHECK(run_line("mat S = [[1, 2], [2, 4]]", symbols).err.empty());

    auto result = run_line("rref S", symbols);
    CHECK(result.err.empty());
    // Should have one pivot row
    CHECK(result.out.find("[ 1, 2 ]") != std::string::npos);
    CHECK(result.out.find("[ 0, 0 ]") != std::string::npos);
}

TEST_CASE("evaluator rref rejects non-matrix") {
    std::unordered_map<std::string, Value> symbols;
    CHECK(run_line("vec v = [1, 2, 3]", symbols).err.empty());

    auto result = run_line("rref v", symbols);
    CHECK(result.out.empty());
    CHECK(result.err.find("rref expects a matrix") != std::string::npos);
}
