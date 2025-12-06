#include "doctest/doctest.h"
#include "la/linear_system.hpp"
#include "la/matrix.hpp"
#include "la/vector.hpp"

TEST_CASE("Number of solutions") {
    using la::Matrix;
    using la::SolutionKind;
    using la::Vector;

    SUBCASE("Unique solution") {
        // clang-format off
        Matrix A(3, 3, {
                           0,  2,  3,
                           2,  3,  1,
                           1, -1, -2
                       });
        // clang-format on
        Vector b({8, 5, -5});

        CHECK_EQ(n_solutions(A, b), SolutionKind::Unique);
    }

    SUBCASE("Infinite solutions") {
        // clang-format off
        Matrix A(3, 4, {
                           1, -1, -1, 2,
                           2, -2, -1, 3,
                           -1,  1, -1, 0
                       });
        // clang-format on
        Vector b({1, 3, -3});
        CHECK_EQ(n_solutions(A, b), SolutionKind::Infinite);
    }

    SUBCASE("Zero solutions") {
        // clang-format off
        Matrix A(3, 3, {
                           1, -1,  2,
                           1,  2, -1,
                           0,  2, -2
                       });
        // clang-format on
        Vector b({3, -3, 1});
        CHECK_EQ(n_solutions(A, b), SolutionKind::None);
    }
}

TEST_CASE("Gaussian elimination") {
    using la::LinearSystemSolution;
    using la::Matrix;
    using la::SolutionKind;
    using la::Vector;

    SUBCASE("Happy case one solution") {
        // Same matrix and vector as in "Number of solutions" subcase "One
        // Solution". Maybe combine this test case with that?
        // clang-format off
        Matrix A(3, 3, {
                           0,  2,  3,
                           2,  3,  1,
                           1, -1, -2
                       });
        // clang-format on
        Vector b({8, 5, -5});

        Vector expected_particular({0, 1, 2});

        auto sol = solve(A, b);
        CHECK(sol.has_solution());
        CHECK(sol.is_unique());
        CHECK(!sol.is_infinite());
        CHECK(sol.directions.size() == 0);
        CHECK_EQ(expected_particular, sol.particular);
    }

    SUBCASE("Happy case no solution") {
        // clang-format off
        Matrix A(3, 3, {
                           1, -1,  2,
                           1,  2, -1,
                           0,  2, -2
                       });
        // clang-format on
        Vector b({3, -3, 1});

        auto sol = solve(A, b);
        CHECK(!sol.has_solution());
        CHECK(!sol.is_unique());
        CHECK(!sol.is_infinite());
        CHECK(sol.directions.size() == 0);
    }

    SUBCASE("Happy case infinite solutions") {
        // Example 2.11 from Poole pp. 74-75
        // clang-format off
        Matrix A(3, 4, {
                           1, -1, -1, 2,
                           2, -2, -1, 3,
                           -1,  1, -1, 0,
                       });
        // Matrix ref_A(3, 4, {
        //   1, -1, -1,  2,
        //   0,  0,  1, -1,
        //   0,  0,  0,  0,
        // });
        Vector b({1, 3, -3});
        // Vector ref_b({1, 1, 0});
        Vector expected_particular({2, 0, 1, 0});
        std::vector<Vector> expected_directions = {{1, 1, 0, 0}, {-1, 0, 1, 1}};

        // clang-format on

        auto sol = solve(A, b);

        CHECK(sol.has_solution());
        CHECK(!sol.is_unique());
        CHECK(sol.is_infinite());
        CHECK_EQ(sol.directions.size(), 2);
        CHECK_EQ(expected_particular, sol.particular);
        CHECK_EQ(expected_directions, sol.directions);
    }
}
