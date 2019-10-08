#include <gtest/gtest.h>
#include <random>
#include <utility>

#include "alloc.h"

namespace {
   std::string vecToStr(const std::vector<int>& v) {
      std::ostringstream s;
      for (int i : v) {
         s << std::to_string(i) << ',';
      }
      return s.str();
   }
}

class AllocTest : public ::testing::TestWithParam<std::pair<std::string, AllocFunc>> {
protected:
};

TEST_P(AllocTest, NonGreedy) {
   // Basic non-greedy algorithm test
   auto testFunction = GetParam().second;
   std::vector<int> input {12, 12, 8, 8, 8};
   EXPECT_EQ(24, testFunction(input.begin(), input.end(), 2));
}

TEST_P(AllocTest, ColFirst) {
   // Breaks row-first algorithms
   auto testFunction = GetParam().second;
   std::vector<int> input {9, 4, 4, 3, 3, 2, 2};
   EXPECT_EQ(9, testFunction(input.begin(), input.end(), 3));
}

TEST_P(AllocTest, RowFirst) {
   // Breaks vertical-first algorithms
   auto testFunction = GetParam().second;
   std::vector<int> input {9, 5, 4, 3, 3, 3};
   EXPECT_EQ(9, testFunction(input.begin(), input.end(), 3));
}

TEST_P(AllocTest, Zero) {
   auto testFunction = GetParam().second;
   std::vector<int> input {};
   EXPECT_EQ(0, testFunction(input.begin(), input.end(), 3));
}

TEST_P(AllocTest, ZeroWorker) {
   auto testFunction = GetParam().second;
   std::vector<int> input {12, 12, 8, 8, 8};
   EXPECT_EQ(std::numeric_limits<int>::max(), testFunction(input.begin(), input.end(), 0));
}

TEST_P(AllocTest, NonClosest) {
   // Breaks the algorithm that tries to pick the closest number to 1/4
   auto testFunction = GetParam().second;
   std::vector<int> input {9, 9, 9, 4, 3, 3, 3};
   EXPECT_EQ(12, testFunction(input.begin(), input.end(), 4));
}

TEST_P(AllocTest, NonClosest2) {
   // Breaks the algorithm above except where it tries to take greater numbers
   auto testFunction = GetParam().second;
   std::vector<int> input {9, 6, 5, 4, 3, 3};
   EXPECT_EQ(11, testFunction(input.begin(), input.end(), 3));
}

TEST_P(AllocTest, NonClosest3) {
   // Breaks the one trying to avoid larger numbers
   auto testFunction = GetParam().second;
   std::vector<int> input {7, 6, 7, 5, 5};
   EXPECT_EQ(12, testFunction(input.begin(), input.end(), 3));
}

TEST_P(AllocTest, RandomData1) {
   auto testFunction = GetParam().second;
   std::vector<int> input {18, 9, 9, 3, 3, 3, 5, 4};
   EXPECT_EQ(18, testFunction(input.begin(), input.end(), 3));
}

TEST(AllocTest, RandomDataForBacktrack) {
   // pregenerate some data
   std::minstd_rand0 rand {};
   std::uniform_int_distribution<int> distribution(1, 10);

   for (int i = 0; i < 100; ++i) {
      std::vector<int> numbers;
      numbers.reserve(8);
      for (int i = 0; i < 10; ++i) {
         numbers.push_back(distribution(rand));
      }
      int ans = bruteForceShortest(numbers.begin(), numbers.end(), 3);
      EXPECT_EQ(ans, backtrackShortest(numbers.begin(), numbers.end(), 3)) << "Failure for numbers " << vecToStr(numbers);
   }
}

INSTANTIATE_TEST_SUITE_P(AllAlgos,
                         AllocTest,
                         testing::Values(
                            std::pair<std::string, AllocFunc> {"BruteForce", bruteForceShortest},
                            std::pair<std::string, AllocFunc> {"Backtracking", backtrackShortest}
                            ),
                         [](auto p) -> std::string { return p.param.first; });

