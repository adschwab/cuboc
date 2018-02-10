#ifndef SECTION_H
#define SECTION_H

#include <cassert>
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
struct BaseSection {
  virtual T get(int x, int y, int z) {
    assert(x < edge);
    assert(y < edge);
    assert(z < edge);
    return T();
  }

  virtual void set(T item, int x, int y, int z) = 0;

  virtual bool isContiguous() { return true; }

  size_t get_edge() { return edge; }
};

template<typename T, size_t edge = temp_utils::pow2<SECTION_SIZE>::value>
struct Section : public BaseSection<T, edge> {
  virtual T get(int x, int y, int z) {
    assert(x < edge);
    assert(y < edge);
    assert(z < edge);
    return arr[x * edge * edge + y * edge + z];
  }

  virtual void set(T item, int x, int y, int z) {
    assert(x < edge);
    assert(y < edge);
    assert(z < edge);
    arr[x * edge * edge + y * edge + z] = item;
  }

  virtual bool isContiguous() { return false; }

  std::array<T, temp_utils::cube<edge>::volume> arr;
};

#endif
