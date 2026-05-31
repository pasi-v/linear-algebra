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

TEST_CASE("evaluator lin_indep returns true for independent vectors") {
    std::unordered_map<std::string, Value> symbols;
    CHECK(run_line("vec a = [1, 0, 0]", symbols).err.empty());
    CHECK(run_line("vec b = [0, 1, 0]", symbols).err.empty());
    CHECK(run_line("vec c = [0, 0, 1]", symbols).err.empty());

    auto result = run_line("lin_indep a b c", symbols);
    CHECK(result.err.empty());
    CHECK(result.out == "true\n");
}

TEST_CASE("evaluator lin_indep returns false for dependent vectors") {
    std::unordered_map<std::string, Value> symbols;
    CHECK(run_line("vec a = [1, 2, 3]", symbols).err.empty());
    CHECK(run_line("vec b = [2, 4, 6]", symbols).err.empty());

    auto result = run_line("lin_indep a b", symbols);
    CHECK(result.err.empty());
    CHECK(result.out == "false\n");
}

TEST_CASE("evaluator lin_indep requires at least one vector") {
    std::unordered_map<std::string, Value> symbols;
    auto result = run_line("lin_indep", symbols);
    CHECK(result.out.empty());
    CHECK(result.err.find("at least one vector") != std::string::npos);
}

TEST_CASE("evaluator lin_indep rejects non-vector arguments") {
    std::unordered_map<std::string, Value> symbols;
    CHECK(run_line("vec a = [1, 0]", symbols).err.empty());
    CHECK(run_line("mat M = [[1, 0], [0, 1]]", symbols).err.empty());

    auto result = run_line("lin_indep a M", symbols);
    CHECK(result.out.empty());
    CHECK(result.err.find("must be a vector") != std::string::npos);
}

TEST_CASE("evaluator lin_indep reports mismatched vector sizes") {
    std::unordered_map<std::string, Value> symbols;
    CHECK(run_line("vec a = [1, 2]", symbols).err.empty());
    CHECK(run_line("vec b = [1, 2, 3]", symbols).err.empty());

    auto result = run_line("lin_indep a b", symbols);
    CHECK(result.out.empty());
    CHECK(!result.err.empty());
}

TEST_CASE("evaluator lin_indep returns true for independent matrices") {
    std::unordered_map<std::string, Value> symbols;
    CHECK(run_line("mat A = [[1, 0], [0, 0]]", symbols).err.empty());
    CHECK(run_line("mat B = [[0, 1], [0, 0]]", symbols).err.empty());
    CHECK(run_line("mat C = [[0, 0], [1, 0]]", symbols).err.empty());

    auto result = run_line("lin_indep A B C", symbols);
    CHECK(result.err.empty());
    CHECK(result.out == "true\n");
}

TEST_CASE("evaluator lin_indep returns false for dependent matrices") {
    std::unordered_map<std::string, Value> symbols;
    CHECK(run_line("mat A = [[1, 2], [3, 4]]", symbols).err.empty());
    CHECK(run_line("mat B = [[2, 4], [6, 8]]", symbols).err.empty());

    auto result = run_line("lin_indep A B", symbols);
    CHECK(result.err.empty());
    CHECK(result.out == "false\n");
}

TEST_CASE("evaluator lin_indep returns true for a single nonzero matrix") {
    std::unordered_map<std::string, Value> symbols;
    CHECK(run_line("mat A = [[1, 0], [0, 0]]", symbols).err.empty());

    auto result = run_line("lin_indep A", symbols);
    CHECK(result.err.empty());
    CHECK(result.out == "true\n");
}

TEST_CASE("evaluator lin_indep returns false for the zero matrix") {
    std::unordered_map<std::string, Value> symbols;
    CHECK(run_line("mat Z = [[0, 0], [0, 0]]", symbols).err.empty());

    auto result = run_line("lin_indep Z", symbols);
    CHECK(result.err.empty());
    CHECK(result.out == "false\n");
}

TEST_CASE("evaluator lin_indep rejects mixing matrices and vectors") {
    std::unordered_map<std::string, Value> symbols;
    CHECK(run_line("mat M = [[1, 0], [0, 1]]", symbols).err.empty());
    CHECK(run_line("vec a = [1, 0]", symbols).err.empty());

    auto result = run_line("lin_indep M a", symbols);
    CHECK(result.out.empty());
    CHECK(result.err.find("cannot mix") != std::string::npos);
}

TEST_CASE("evaluator lin_indep reports mismatched matrix sizes") {
    std::unordered_map<std::string, Value> symbols;
    CHECK(run_line("mat A = [[1, 0], [0, 1]]", symbols).err.empty());
    CHECK(run_line("mat B = [[1, 0, 0], [0, 1, 0]]", symbols).err.empty());

    auto result = run_line("lin_indep A B", symbols);
    CHECK(result.out.empty());
    CHECK(!result.err.empty());
}
