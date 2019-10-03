#include <gtest/gtest.h>
#include <utility>

#include "alloc.h"

class AllocTest : public ::testing::TestWithParam<std::pair<std::string, AllocFunc>> {
protected:
};

TEST_P(AllocTest, NonGreedy) {
   auto testFunction = GetParam().second;
   EXPECT_EQ(24, testFunction({12, 12, 8, 8, 8}, 2));
}

TEST_P(AllocTest, PerRow) {
   auto testFunction = GetParam().second;
   EXPECT_EQ(9, testFunction({9, 4, 4, 3, 3, 2, 2}, 3));
}

TEST_P(AllocTest, PerWorker) {
   auto testFunction = GetParam().second;
   EXPECT_EQ(9, testFunction({9, 5, 4, 3, 3, 3}, 3));
}

TEST_P(AllocTest, Zero) {
   auto testFunction = GetParam().second;
   EXPECT_EQ(0, testFunction({}, 3));
}

TEST_P(AllocTest, Complex1) {
   auto testFunction = GetParam().second;
   EXPECT_EQ(18, testFunction({18, 9, 9, 3, 3, 3, 5, 4}, 3));
}

INSTANTIATE_TEST_CASE_P(AllocTests,
                        AllocTest,
                        testing::Values(
                           std::pair<std::string, AllocFunc> {"BruteForce", bruteForceShortest},
                           std::pair<std::string, AllocFunc> {"Backtracking", backtrackShortest}
                           ),
                        [](auto p) -> std::string { return p.param.first; });

