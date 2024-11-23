#include <benchmark/benchmark.h>

void Sample(benchmark::State& state) {
  for (auto _ : state) {
    auto result = 2 + 2;
    benchmark::DoNotOptimize(result);
  }
}

BENCHMARK(Sample);
BENCHMARK_MAIN();
