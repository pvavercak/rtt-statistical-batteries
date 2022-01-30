#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <climits>
#include <compare>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <vector>

namespace bsi {
namespace utils {
inline uint8_t char_to_bit(const char bit) { return bit - '0'; }
inline size_t chars_to_number(const char *chars, const size_t num_chars) {
  size_t number{0};
  for (size_t i = 0; i < num_chars; ++i) {
    number += char_to_bit(chars[i]) << (num_chars - 1 - i);
  }
  return number;
}
} // namespace utils

/// t0_words
void words_test(const std::vector<unsigned char> &input_sequence) {
  auto input_ptr = input_sequence.data();
  constexpr size_t ArraySize{65536};

  auto inputLen = input_sequence.size() / CHAR_BIT;
  if (inputLen < ArraySize * 6) {
    throw std::runtime_error("Invalid input sequence");
  }

  std::vector<size_t> storage(ArraySize, 0);

  size_t failed{0};
  std::array<char, 8> temp{};
  std::fill(temp.begin(), temp.end(), 0);
  auto storage_ptr = storage.data();
  const auto tmp_ptr = reinterpret_cast<size_t *>(temp.data());
  const auto num_iterations = inputLen / (ArraySize * 6);
  for (size_t iteration = 0; iteration < num_iterations; iteration++) {
    for (size_t i = 0; i < ArraySize; i++) {
      for (size_t j = 0; j < 6; j++) {
        temp[j] = input_ptr[i * 6 + j];
      }
      storage_ptr[i] = *tmp_ptr;
    }

    std::sort(storage_ptr, storage_ptr + ArraySize, std::less<decltype(storage_ptr[0])>());

    for (size_t i = 0; i < ArraySize - 1; i++) {
      if (storage_ptr[i] == storage_ptr[i + 1]) {
        failed++;
        break;
      }
    }
  }

  std::cout << "Words failed: " << failed << std::endl;
}

size_t do_monobit_test(const unsigned char *seq) {
  size_t sum{0};
  for (size_t i = 0; i < 20000; ++i) {
    sum += *seq;
    ++seq;
  }

  return sum;
}

/// t1_monobit
void monobit_test(const std::vector<unsigned char> &input_sequence) {
  std::array<size_t, 257> results{};
  const auto input_size = input_sequence.size();
  if (input_size < 5140000) {
    throw std::runtime_error("Invalid size of input sequence");
  }

  auto *seq = input_sequence.data();
  const size_t sequence_stride{20000};
  size_t num_failed{0};
  for (auto &&result : results) {
    result = do_monobit_test(seq);
    if (result < 9654 || result > 10346) {
      ++num_failed;
    }
    seq += sequence_stride;
  }

  std::cout << "Monobit failed: " << num_failed << std::endl;
}

double do_poker_test(const unsigned char *seq) {
  int number{0};
  double x{0.0};
  std::array<int, 16> numoccur{};
  std::fill(numoccur.begin(), numoccur.end(), 0);

  // 20000 bits, 1 number consists from 4 bits => 5000 iterations
  for (size_t i = 0; i < 5000; i++) {
    number = ((seq[0]) << 3) + ((seq[1]) << 2) + ((seq[2]) << 1) + ((seq[3]) << 0);
    seq += 4;
    numoccur[number]++;
  }

  for (const auto &number : numoccur) {
    x += std::pow(number, 2);
  }

  x = x * (16.0 / 5000.0) - 5000;

  return x;
}

/// t2_poker
void poker_test(const std::vector<unsigned char> &input_sequence) {
  int inputLen = 0, i, failed;
  std::array<double, 257> results{};
  auto seq = input_sequence.data();

  const auto input_size = input_sequence.size();
  if (input_size < 5140000) {
    throw std::runtime_error("Invalid size of input sequence");
  }

  std::fill(results.begin(), results.end(), 0.0);
  const size_t sequence_stride{20000};
  size_t num_failed{0};
  // do T2 evaluation for 257 sequences, each consists from 20 000 bits (2500
  // Bytes)
  for (auto &&result : results) {
    result = do_poker_test(seq);
    if ((result < 1.03) || (result > 57.4))
      num_failed++;
    seq += sequence_stride;
  }

  std::cout << "Poker failed: " << num_failed << std::endl;
}

char do_runs_test(const unsigned char *seq) {
  constexpr size_t MaxRunLength{6};
  constexpr std::array<size_t, MaxRunLength> lnruns{2267, 1079, 502, 223, 90, 90};
  constexpr std::array<size_t, MaxRunLength> hnruns{2733, 1421, 748, 402, 223, 223};

  std::array<size_t, MaxRunLength> numruns0{};
  std::fill(numruns0.begin(), numruns0.end(), 0);
  std::array<size_t, MaxRunLength> numruns1{};
  std::fill(numruns1.begin(), numruns1.end(), 0);

  uint8_t current_bit{*seq++};
  int current_run_length{1};
  for (size_t i = 1; i < 20000; i++) {
    if (*seq++ == current_bit) {
      ++current_run_length;
    } else {
      if (current_run_length > MaxRunLength) {
        current_run_length = MaxRunLength;
      }
      if (current_bit == 1) {
        numruns1[current_run_length - 1]++;
      } else {
        numruns0[current_run_length - 1]++;
      }
      current_bit = *(seq - 1);
      current_run_length = 1;
    }
  }

  // encounter the last run of bits
  if (current_run_length > MaxRunLength) {
    current_run_length = MaxRunLength;
  }
  if (current_bit == 1) {
    numruns1[current_run_length - 1]++;
  } else {
    numruns0[current_run_length - 1]++;
  }
  // evaluation
  size_t failed{0};
  for (size_t i = 0; i < MaxRunLength; i++) {
    if ((numruns0[i] < lnruns[i]) || (numruns0[i] > hnruns[i]))
      failed++;
    if ((numruns1[i] < lnruns[i]) || (numruns1[i] > hnruns[i]))
      failed++;
  }

  return failed;
}

/// tbsi_t3_runs
void runs_test(const std::vector<unsigned char> &input_sequence) {
  auto seq = input_sequence.data();
  std::array<char, 257> results{};
  std::fill(results.begin(), results.end(), 0);

  // test of input length, minimal is 5 140 000 bits (642 500 Bytes)
  const auto input_size = input_sequence.size(); // size of input string is in bits!!!
  if (input_size < 5140000) {
    throw std::runtime_error("Invalid size of input sequence");
  }
  size_t failed{0};
  const size_t sequence_stride{20000};
  for (auto &&result : results) {
    result = do_runs_test(seq);
    if (result > 0) {
      ++failed;
    }
    seq += sequence_stride;
  }
  std::cout << "Runs failed: " << failed << std::endl;
}

/// @returns This function returns TRUE in case of a failure
bool do_long_run_test(const unsigned char *seq) {
  uint8_t current_bit = *seq++;
  size_t current_run_length = 1;
  size_t max_run_length = 1;
  for (size_t i = 1; i < 20000; i++) {
    if (*seq++ == current_bit) {
      ++current_run_length;
    } else {
      if (current_run_length > max_run_length) {
        max_run_length = current_run_length;
      }
      current_bit = *(seq - 1);
      current_run_length = 1;
    }
  }
  if (current_run_length > max_run_length) {
    max_run_length = current_run_length;
  }
  return max_run_length >= 34;
}

void long_run_test(const std::vector<unsigned char> &input_sequence) {
  auto seq = input_sequence.data();
  std::array<bool, 257> results{};
  std::fill(results.begin(), results.end(), 0);

  const auto input_size = input_sequence.size();
  if (input_size < 5140000) {
    throw std::runtime_error("Invalid size of input sequence");
  }

  size_t failed{0};
  for (auto &&result : results) {
    result = do_long_run_test(seq);
    if (result) {
      ++failed;
    }
    seq += 20000;
  }

  std::cout << "Long Run failed: " << failed << std::endl;
}

typedef struct data_s {
  short Z;
  short tau;
} data_t;

short do_autocorrelation_test(const unsigned char *seq, std::array<data_t, 5000> &data) {
  int ind, tau, numMaxTau;
  short xorSum, maxZ;
  auto origSeq = seq;
  for (tau = 1; tau <= 5000; tau++) {
    xorSum = 0;
    seq = origSeq;
    for (ind = 0; ind < 5000; ind++) {
      xorSum += *seq ^ *(seq + tau);
      seq++;
    }

    xorSum -= 2500;
    if (xorSum < 0)
      xorSum = -xorSum;
    data[tau - 1].Z = xorSum;
    data[tau - 1].tau = tau;
  }
  std::sort(data.begin(), data.end(), [](const auto &lhs, const auto &rhs) { return lhs.Z < rhs.Z; });
  maxZ = data[0].Z;
  numMaxTau = 1;
  while (data[numMaxTau].Z == maxZ)
    numMaxTau++;
  ind = rand() % numMaxTau;
  tau = data[ind].tau;
  seq = origSeq + 10000;
  xorSum = 0;
  for (ind = 0; ind < 5000; ind++) {
    xorSum += *seq ^ *(seq + tau);
    seq++;
  }
  return xorSum;
}

void autocorrelation_test(const std::vector<unsigned char> &input_sequence) {
  auto seq = input_sequence.data();
  std::array<short, 257> results{};
  std::fill(results.begin(), results.end(), 0);
  const auto input_size = input_sequence.size();
  if (input_size < 5140000) {
    throw std::runtime_error("Invalid size of input sequence");
  }
  srand(time(NULL));
  std::array<data_t, 5000> data{};
  std::fill(data.begin(), data.end(), data_t{0, 0});
  size_t failed{0};
  for (auto &&result : results) {
    result = do_autocorrelation_test(seq, data);
    if (result < 2326 || result > 2674) {
      ++failed;
    }
    seq += 20000;
  }

  std::cout << "Autocorellation failed: " << failed << std::endl;
}

int do_uniform_test(const unsigned char *seq, size_t *X, const size_t K, const size_t N, const double A) {
  size_t w{0};
  for (size_t i = 0; i < N; i++) {
    w = 0;
    for (size_t j = 0; j < K; j++) {
      w <<= 1;
      w |= *seq++;
    }
    X[w]++;
  }
  const auto power = std::pow(2.0, -K);
  const auto low = power - A;
  const auto high = power + A;

  size_t failed{0};
  double ratio{0.0};
  for (size_t i = 0; i < (1ull << K); i++) {
    ratio = static_cast<double>(X[i]) / static_cast<double>(N);
    if (ratio < low || ratio > high) {
      failed++;
    }
  }

  return failed;
}

void uniform_test(const std::vector<unsigned char> &input_sequence, const size_t K, const size_t N, const double A) {
  auto seq = input_sequence.data();
  const auto input_size = input_sequence.size();
  if (K * N > input_size) {
    throw std::runtime_error("Required length of the sequence to be tested is too large");
  }
  if (K < 1 || K > 24) {
    throw std::runtime_error("Length of the vectors to be tested must be in [1;24]");
  }
  if (A <= 0 || A >= 1) {
    throw std::runtime_error("Parameter A must be in (0;1)");
  }

  size_t size{1ull << K};
  std::vector<size_t> X(size, 0);

  size_t failed{0};
  const auto iterations = input_size / (N * K);
  for (size_t i = 0; i < iterations; i++) {
    std::fill(X.begin(), X.end(), 0);
    if (do_uniform_test(seq, X.data(), K, N, A) > 0) {
      ++failed;
    }
    seq += K * N;
  }
  std::cout << "Uniform failed: " << failed << std::endl;
}

} // namespace bsi
