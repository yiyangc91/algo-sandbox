#include <random>
#include <vector>
#include <benchmark/benchmark.h>

#include "alloc.h"

static constexpr int maxElems = 8;
static constexpr int maxWorkers = 4;
static constexpr int maxDistribution = 16;

static void BM_simple(benchmark::State& state, AllocFunc func) {
   // pregenerate some data
   std::minstd_rand0 rand {};
   std::uniform_int_distribution<int> distribution(1, maxDistribution);
   std::vector<int> numbers;
   numbers.reserve(maxElems);
   for (int i = 0; i < maxElems; ++i) {
      numbers.push_back(distribution(rand));
   }

   // Perform setup here
   for (auto _ : state) {
      // This code gets timed
      func(numbers.begin(), numbers.begin()+state.range(0), state.range(1));
   }
}

int main(int argc, char** argv) {
   for (auto& test_input : {
        std::pair<std::string, AllocFunc> {"bruteforce", bruteForceShortest},
        std::pair<std::string, AllocFunc> {"backtrack", backtrackShortest} }) {
      benchmark::RegisterBenchmark(test_input.first.c_str(), BM_simple, test_input.second)
            ->RangeMultiplier(2)
            ->Ranges({{1, maxElems}, {1, maxWorkers}})
            ->Complexity();
   }
   benchmark::Initialize(&argc, argv);
   benchmark::RunSpecifiedBenchmarks();
}
