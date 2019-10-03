#include <benchmark/benchmark.h>
#include <random>
#include <chrono>
#include "impl/base.h"
#include "generate/testgen.h"

static constexpr int BENCH_X_START = 2;
static constexpr int BENCH_Y_START = 0;
static constexpr int BENCH_X_MAX = 3;
static constexpr int BENCH_Y_MAX = 5;
static constexpr int BENCH_X_GROWTH = 8 << 8;
static constexpr int BENCH_Y_GROWTH = 2;
static constexpr int BENCH_X_INITIAL = 8;
static constexpr int BENCH_Y_INITIAL = 1;

void BM_test(benchmark::State& state, BaseSearch* impl, const std::pair<std::string, std::string> (*sequences)[BENCH_X_MAX]) {
   for (auto _ : state) {
      auto& pair = sequences[state.range(0)][state.range(1)];
      (*impl)(pair.first, pair.second);
   }
}

int main(int argc, char** argv) {
   auto seed = std::chrono::system_clock::now().time_since_epoch().count();
   std::pair<std::string, std::string> matrix[BENCH_Y_MAX][BENCH_X_MAX];
   int ip = BENCH_Y_INITIAL;
   for (int i = 0; i < BENCH_Y_MAX; ++i) {
      int jp = BENCH_X_INITIAL;
      for (int j = 0; j < BENCH_X_MAX; ++j) {
         TestGen<std::minstd_rand0> testGen { seed, static_cast<std::string::size_type>(ip), static_cast<std::string::size_type>(jp) };
         auto seqs = testGen.generateStringSequences();
         matrix[i][j] = {std::move(seqs.first), std::move(seqs.second)};
         jp *= BENCH_X_GROWTH;
      }
      ip *= BENCH_Y_GROWTH;
   }

   for (auto& testInput : getInstance()->getInstances()) {
      auto registered = benchmark::RegisterBenchmark(testInput.first.c_str(), BM_test, testInput.second, matrix)
            ->ArgNames({"Needle", "Haystack"});
      for (int i = BENCH_Y_START; i < BENCH_Y_MAX; ++i) {
         for (int j = BENCH_X_START; j < BENCH_X_MAX; ++j) {
            registered->ArgPair(i, j);
         }
      }
   }

   benchmark::Initialize(&argc, argv);
   benchmark::RunSpecifiedBenchmarks();
}
