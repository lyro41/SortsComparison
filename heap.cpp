#include "heap.h"

heap::heap(std::vector<uint32_t> &data_set) 
{
  this->data = data_set;
  this->n = data_set.size();
  for (int i = n << 1; i >= 0; --i) {
    this->heapify(i);
  }
  return;
}

uint32_t heap::size() {
  return this->n;
}

void heap::heapify(uint32_t pos) {
  uint32_t largest, left, right;
  for (uint32_t i = pos;;) {
    largest = i;
    left = (i << 1) + 1;
    right = left + 1;

    if (left < this->n && this->data[largest] < this->data[left]) {
      largest = left;
    }

    if (right < this->n && this->data[largest] < this->data[right]) {
      largest = right;
    }

    if (largest == i) {
      break;
    }

    std::swap(this->data[i], this->data[largest]);
    i = largest;
  }
}

void heap::insert(uint32_t elem) {
  this->data.push_back(elem);
  uint32_t parent = (this->n - 1) >> 1;
  for (uint32_t i = this->n; i > 0 && data[i] > data[parent]; ) {
    std::swap(data[parent], data[i]);
    i = parent;
    parent = (i - 1) >> 1;
  }
  ++this->n;
  return;
}

uint32_t heap::top() {
  uint32_t output = this->data[0];
  this->data[0] = this->data[this->n - 1];
  //this->data.pop_back();
  --this->n;
  return output;
}
