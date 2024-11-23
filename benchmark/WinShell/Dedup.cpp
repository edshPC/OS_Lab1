#include <benchmark/benchmark.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <unordered_set>

namespace {

const size_t RAMSize = 64 * (1ULL << 20ULL) / sizeof(int64_t);  // 64 Mb

void prepeare(std::vector<int64_t>& array) {
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<int64_t> dis(1, RAMSize / 2);
  for (size_t i = 0; i < RAMSize; ++i) {
    array[i] = dis(gen);
  }
  std::cout << "Prepeared array: " << RAMSize << '\n';
}

size_t compute(std::vector<int64_t>& array) {
  std::unordered_set<int64_t> seen;
  std::vector<int64_t> unique_elements;

  for (const auto& elem : array) {
    if (seen.insert(elem).second) {
      unique_elements.push_back(elem);
    }
  }

  array = std::move(unique_elements);
  return array.size();
}

}  // namespace

class MyFixture : public benchmark::Fixture {
  std::vector<int64_t> array;
public:
  MyFixture() {
    array = std::vector<int64_t>(RAMSize);
    prepeare(array);
    Unit(benchmark::kMillisecond);
  }
  std::vector<int64_t>& getArray() { return array; }
};

BENCHMARK_F(MyFixture, Main)(benchmark::State& state) {  // NOLINT
  for (auto st_val : state) {
    auto result = compute(getArray());
    benchmark::DoNotOptimize(result);
    std::cout << "Unique values left: " << result << '\n';
  }
}

BENCHMARK_MAIN();  // NOLINT
