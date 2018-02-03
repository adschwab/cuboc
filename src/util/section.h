#ifndef SECTION_H
#define SECTION_H

#include <array>

#define SECTION_SIZE 2

namespace temp_utils {

template<size_t size>
struct pow2 {
  static const size_t value = 1 << size;
};

template<size_t edge>
struct cube {
  static const size_t volume = edge * edge * edge;
};

}

template<typename T, size_t edge = temp_utils::pow2<SECTION_SIZE>::value>
struct Section {
  T get(int x, int y, int z) {
    return arr[x * edge * edge + y * edge + z];
  }

  void set(T item, int x, int y, int z) {
    arr[x * edge * edge + y * edge + z] = item;
  }

  std::array<T, temp_utils::cube<edge>::volume> arr;
};
#endif
