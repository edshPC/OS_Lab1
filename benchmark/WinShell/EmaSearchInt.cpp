#include <benchmark/benchmark.h>
#include <corecrt_io.h>
#include <fcntl.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>

namespace {

const size_t FileSize = 256 * (1ULL << 20ULL) / sizeof(int64_t);  // 256 Mb
const size_t RAMSize = FileSize / 8;
const int64_t ToFind = -777;

void prepeare() {
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<int64_t> dis(1, FileSize - 1);

  std::vector<int64_t> output(FileSize);

  for (size_t i = 0; i < FileSize; ++i) {
    output[i] = dis(gen);
  }
  const size_t place = dis(gen);
  output[place] = ToFind;  // Найти это значение в случайном месте

  std::ofstream out_file("input.bin", std::ios::binary | std::ios::trunc);
  if (!out_file) {
    throw std::runtime_error("Failed to open output file");
  }
  out_file.write(reinterpret_cast<const char*>(output.data()), sizeof(int64_t) * FileSize);
  out_file.close();
  std::cout << "File size num " << FileSize << '\n';
  std::cout << "RAM size num " << RAMSize << '\n';
  std::cout << "Value placed at " << place << '\n';
}

size_t compute() {
  int fd = -1;
  _sopen_s(&fd, "input.bin", _O_RDONLY | _O_BINARY,_SH_DENYNO,_S_IREAD);
  //fd = open("input.bin", O_RDONLY | O_BINARY);
  if (fd < 0) {
    throw std::runtime_error("Failed to open input file");
  }

  size_t found = 0;
  size_t block = 0;
  std::vector<int64_t> buffer(RAMSize);

  while (found == 0) {
    const size_t num_read = _read(fd, buffer.data(), sizeof(int64_t) * RAMSize) / sizeof(int64_t);
    benchmark::DoNotOptimize(num_read);
    if (num_read < 0) {
      _close(fd);
      throw std::runtime_error("Error reading from file");
    }
    if (num_read == 0) {
      break;
    }

    for (volatile size_t i = 0; i < num_read; ++i) {
      if (buffer[i] == ToFind) {
        found = RAMSize * block + i;
        break;
      }
    }
    block++;
  }
  _close(fd);
  std::cout << "found at " << found << " (block " << block << ")" << '\n';
  return found;
}

}  // namespace

class MyFixture : public benchmark::Fixture {
public:
  MyFixture() {
    //prepeare();
    Unit(benchmark::kMillisecond);
  }
};

BENCHMARK_F(MyFixture, Main)(benchmark::State& state) { // NOLINT
  for (auto st_val : state) {
    auto result = compute();
    benchmark::DoNotOptimize(result);
  }
}

BENCHMARK_MAIN();  // NOLINT
