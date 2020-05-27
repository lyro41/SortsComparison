#include <iostream>
#include <chrono>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <queue>
#include "heap.h"

int Comp(const void *first, const void *second) {
  return *static_cast<const uint32_t *>(first) -
         *static_cast<const uint32_t *>(second);
}

void Merge(std::vector<uint32_t>::iterator first_begin,
           std::vector<uint32_t>::iterator first_end,
           std::vector<uint32_t>::iterator second_begin,
           std::vector<uint32_t>::iterator second_end,
           std::vector<uint32_t>::iterator buff) {
  int64_t size = second_end - first_begin;
  for (;;) {
    if (first_begin == first_end) {
      while (second_begin != second_end) {
        *buff = *second_begin;
        ++buff;
        ++second_begin;
      }
      break;
    } else if (second_begin == second_end) {
      while (first_begin != first_end) {
        *buff = *first_begin;
        ++buff;
        ++first_begin;
      }
      break;
    }
    if (*first_begin <= *second_begin) {
      *buff = *first_begin;
      ++first_begin;
    } else {
      *buff = *second_begin;
      ++second_begin;
    }
    ++buff;
  }
  buff -= size;
  second_begin -= size;
  while (second_begin != second_end) {
    *second_begin = *buff;
    ++second_begin;
    ++buff;
  }
  return;
}

void MergeSort(std::vector<uint32_t>::iterator begin,
               std::vector<uint32_t>::iterator end,
               std::vector<uint32_t>::iterator buff) {
  if (end - begin == 1) return;
  int64_t shift = (end + 1 - begin) >> 1;
  MergeSort(begin, begin + shift, buff);
  MergeSort(begin + shift, end, buff);
  Merge(begin, begin + shift, begin + shift, end, buff);
  return;
}

void MergeSort(std::vector<uint32_t>::iterator begin,
               std::vector<uint32_t>::iterator end) {
  std::vector<uint32_t> buff(end - begin);
  MergeSort(begin, end, buff.begin());
  return;
}

void RadixSort(std::vector<uint32_t>::iterator begin,
               std::vector<uint32_t>::iterator end,
               uint32_t size) {
  std::vector<std::vector<uint32_t>> buff(256);
  std::vector<uint32_t>::iterator elem;
  for (uint32_t i = 0; i < size; ++i) {
    elem = begin;
    for (uint8_t *now = reinterpret_cast<uint8_t *>(&*begin) + i;
         elem != end; now += 4, ++elem) {
      buff[*now].push_back(*elem);
    }
    elem = begin;
    for (std::vector<std::vector<uint32_t>>::iterator byte = buff.begin();
         byte != buff.end(); ++byte) {
      for (std::vector<uint32_t>::iterator value = byte->begin();
           value != byte->end(); ++value) {
        *elem = *value;
        ++elem;
      }
      byte->resize(0);
    }
  }
  return;
}

void TwoByteRadixSort(std::vector<uint32_t>::iterator begin,
                      std::vector<uint32_t>::iterator end,
                      uint32_t size) {
  std::vector<std::vector<uint32_t>> buff(65536);
  std::vector<uint32_t>::iterator elem;
  size >>= 1;
  for (uint32_t i = 0; i < size; ++i) {
    elem = begin;
    for (uint16_t *now = reinterpret_cast<uint16_t *>(&*begin) + i;
         elem != end; now += 2, ++elem) {
      buff[*now].push_back(*elem);
    }
    elem = begin;
    for (std::vector<std::vector<uint32_t>>::iterator byte = buff.begin();
         byte != buff.end(); ++byte) {
      for (std::vector<uint32_t>::iterator value = byte->begin();
           value != byte->end(); ++value) {
        *elem = *value;
        ++elem;
      }
      byte->resize(0);
    }
  }
  return;
}

void ShellSort(std::vector<uint32_t>::iterator begin,
               std::vector<uint32_t>::iterator end) {
  uint32_t buff;
  std::vector<uint32_t>::iterator suspect;
  for (uint32_t gap = end - begin >> 1; gap > 0; gap >>= 1) {
    for (std::vector<uint32_t>::iterator now = begin + gap; now != end; ++now) {
      buff = *now;
      for (suspect = now;
           suspect >= begin + gap && *(suspect - gap) > buff;  suspect -= gap) {
        *suspect = *(suspect - gap);
      }
      *suspect = buff;
    }
  }
  return;
}

void ShellSort(uint32_t *begin,
               uint32_t *end) {
  uint32_t buff;
  uint32_t *suspect;
  for (uint32_t gap = end - begin >> 1; gap > 0; gap >>= 1) {
    for (uint32_t *now = begin + gap; now != end; ++now) {
      buff = *now;
      for (suspect = now;
           suspect >= begin + gap && *(suspect - gap) > buff;  suspect -= gap) {
        *suspect = *(suspect - gap);
      }
      *suspect = buff;
    }
  }
  return;
}

void HeapSort(std::vector<uint32_t> &data) {
  heap buff(data);
  std::vector<uint32_t>::iterator i = data.begin();
  while (i != data.end()) {
    *i = buff.top();
    buff.heapify(0);
    ++i;
  }
}

uint32_t Middle(uint32_t a, uint32_t b, uint32_t c) {
  if (a < b) {
    if (b < c) {
      return b;
    } else {
      if (a < c) {
        return c;
      } else {
        return a;
      }
    }
  } else {
    if (b < c) {
      if (a < c) {
        return a;
      } else {
        return c;
      }
    } else {
      return b;
    }
  }
}

void RosSort(uint32_t *begin, uint32_t *end) {
  if (end - begin <= 32) {
    ShellSort(begin, end);
    return;
  }
  uint32_t *lhs = begin - 1;
  uint32_t *rhs = end;
  {
  std::srand(std::time(nullptr));
  uint32_t pivot = Middle(*(begin + std::abs(std::rand()) % (end - begin - 2) + 1),
                         *begin, *(end - 1));
  uint32_t *equal_lhs = begin - 1;
  uint32_t *equal_rhs = end;
  for (;;) {
    while (*++lhs < pivot);

    while (*--rhs > pivot) if (rhs == begin) break;

    if (lhs >= rhs) break;

    std::swap(*lhs, *rhs);

    if (*lhs == pivot) std::swap(*lhs, *++equal_lhs);
    if (*rhs == pivot) std::swap(*rhs, *--equal_rhs);
  }
  while (equal_rhs != end) {
    std::swap(*lhs, *equal_rhs);
    ++lhs;
    ++equal_rhs;
  }
  while (equal_lhs >= begin) {
    std::swap(*rhs, *equal_lhs);
    --rhs;
    --equal_lhs;
  }
  }
  RosSort(begin, rhs + 1);
  RosSort(lhs, end);
}

void Checker() {
  //std::experimental::reseed(std::time(nullptr));
  std::srand(std::time(nullptr));
  std::vector<std::vector<uint32_t>> random(49), data(49);
  std::vector<std::vector<uint8_t>> random_short(49), data_short(49);
  //std::vector<std::vector<uint32_t>> random_int(49), data_int(49);
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  std::chrono::time_point<std::chrono::high_resolution_clock> end;
  std::ofstream outstream;

  for (uint8_t i = 0; i < 7; ++i) {
    random[7 * i].resize(13);
    random[7 * i + 1].resize(97);
    random[7 * i + 2].resize(249);
    random[7 * i + 3].resize(1999);
    random[7 * i + 4].resize(37931);
    random[7 * i + 5].resize(512512);
    random[7 * i + 6].resize(7003007);
    random_short[7 * i].resize(13);
    random_short[7 * i + 1].resize(97);
    random_short[7 * i + 2].resize(249);
    random_short[7 * i + 3].resize(1999);
    random_short[7 * i + 4].resize(37931);
    random_short[7 * i + 5].resize(512512);
    random_short[7 * i + 6].resize(7003007);
  }
  for (uint32_t i = 0; i < random.size(); ++i) {
    for (uint32_t j = 0; j < random[i].size(); ++j) {
      random[i][j] = std::rand() % 10000000007;
      random_short[i][j] = std::rand() % 65536;
    }
  }

  //data_short = random_short;
  //for (int i = 0; i < 49; ++i) {
  //  RosSort(&*data_short[i].begin(), &*data_short[i].end());
  //
  //}

  //goto start_point;

  outstream << std::fixed << std::setprecision(3);
  outstream.open("D:/YandexDisk/sorts_data/Data_QTx32_fixed.csv");

  outstream << "\nRostiSort\n";
  data = random;
  for (uint8_t i = 0; i < 7; ++i) {
    for (uint8_t j = 0; j < data.size() / 7; ++j) {
      start = std::chrono::high_resolution_clock::now();
      RosSort(&*data[j * 7 + i].begin(), &*data[j * 7 + i].end());
      end = std::chrono::high_resolution_clock::now();
      outstream << std::setfill(' ') << std::setw(9)
                << std::chrono::duration<double, std::milli>
                   (end - start).count() << ',';
    }
    outstream << '\n';
  }

  outstream.close();
  return;

  // 2Byte for int
  outstream << "\n32-bit Two Byte Radix Sort\n";
  data = random;
  for (uint8_t i = 0; i < 7; ++i) {
    for (uint8_t j = 0; j < data.size() / 7; ++j) {
      start = std::chrono::high_resolution_clock::now();
      TwoByteRadixSort(data[j * 7 + i].begin(), data[j * 7 + i].end(), 4);
      end = std::chrono::high_resolution_clock::now();
      outstream << std::setfill(' ') << std::setw(9)
                << std::chrono::duration<double, std::milli>
                   (end - start).count() << ',';
    }
    outstream << '\n';
  }

  // for int
  outstream << "\n32-bit Byte Radix Sort\n";
  data = random;
  for (uint8_t i = 0; i < 7; ++i) {
    for (uint8_t j = 0; j < data.size() / 7; ++j) {
      start = std::chrono::high_resolution_clock::now();
      RadixSort(data[j * 7 + i].begin(), data[j * 7 + i].end(), 4);
      end = std::chrono::high_resolution_clock::now();
      outstream << std::setfill(' ') << std::setw(9)
                << std::chrono::duration<double, std::milli>
                   (end - start).count() << ',';
    }
    outstream << '\n';
  }

  data = random;
  outstream << "\nMerge Sort\n";
  for (uint8_t i = 0; i < 7; ++i) {
    for (uint8_t j = 0; j < data.size() / 7; ++j) {
      start = std::chrono::high_resolution_clock::now();
      MergeSort(data[j * 7 + i].begin(), data[j * 7 + i].end());
      end = std::chrono::high_resolution_clock::now();
      outstream << std::setfill(' ') << std::setw(9)
                << std::chrono::duration<double, std::milli>
                   (end - start).count() << ',';
    }
    outstream << '\n';
  }

  data = random;
  outstream << "\nC++ std::sort\n";
  for (uint8_t i = 0; i < 7; ++i) {
    for (uint8_t j = 0; j < data.size() / 7; ++j) {
      start = std::chrono::high_resolution_clock::now();
      std::sort(data[j * 7 + i].begin(), data[j * 7 + i].end());
      end = std::chrono::high_resolution_clock::now();
      outstream << std::setfill(' ') << std::setw(9)
                << std::chrono::duration<double, std::milli>
                   (end - start).count() << ',';
    }
    outstream << '\n';
  }

  data = random;
  outstream << "\nC++ std::qsort\n";
  for (uint8_t i = 0; i < 7; ++i) {
    for (uint8_t j = 0; j < data.size() / 7; ++j) {
      start = std::chrono::high_resolution_clock::now();
      std::qsort(&*data[j * 7 + i].begin(), data[j * 7 + i].size(), sizeof(uint32_t), Comp);
      end = std::chrono::high_resolution_clock::now();
      outstream << std::setfill(' ') << std::setw(9)
                << std::chrono::duration<double, std::milli>
                   (end - start).count() << ',';
    }
    outstream << '\n';
  }

  data = random;
  outstream << "\nShell Sort\n";
  for (uint8_t i = 0; i < 7; ++i) {
    for (uint8_t j = 0; j < data.size() / 7; ++j) {
      start = std::chrono::high_resolution_clock::now();
      ShellSort(data[j * 7 + i].begin(), data[j * 7 + i].end());
      end = std::chrono::high_resolution_clock::now();
      outstream << std::setfill(' ') << std::setw(9)
                << std::chrono::duration<double, std::milli>
                   (end - start).count() << ',';
    }
    outstream << '\n';
  }

  //start_point:

  data = random;
  outstream << "\nHeap Sort\n";
  for (uint8_t i = 0; i < 7; ++i) {
    for (uint8_t j = 0; j < data.size() / 7; ++j) {
      start = std::chrono::high_resolution_clock::now();
      HeapSort(data[j * 7 + i]);
      end = std::chrono::high_resolution_clock::now();
      outstream << std::setfill(' ') << std::setw(9)
                << std::chrono::duration<double, std::milli>
                   (end - start).count() << ',';
    }
    outstream << '\n';
  }

  outstream.close();

}

int main() {
  Checker();
  return 0;
}
