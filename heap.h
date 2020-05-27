#pragma once
#ifndef HEAP_H
#define HEAP_H
#include <iostream>
#include <vector>

class heap {
 private:
  uint64_t n;
  std::vector<uint32_t> data;

 public:
  heap(std::vector<uint32_t> &data_set);
  void insert(uint32_t elem);
  void heapify(uint32_t pos);
  uint32_t size();
  uint32_t top();

};

#endif // HEAP_H
