#ifndef CACHE_H
#define CACHE_H

#include <vector>

#define DEFAULT_SIZE 10

namespace util {

template<typename I, typename D>
class Cache {
  public:
  Cache(int size=DEFAULT_SIZE) :
      _size(size) {
    _cache = std::vector<std::shared_ptr<D> >(size);
    _addrs = std::vector<I>(size);
  }

  bool contains(I index) {
    return index == _addrs[hash_fn(index) % _size];
  }

  std::shared_ptr<D> get(I index) {
    return _cache[hash_fn(index) % _size];
  }

  void add(I index, std::shared_ptr<D> data) {
    _cache[hash_fn(index) % _size] = data;
  }

  private:
  int _size;
  std::vector<std::shared_ptr<D> > _cache;
  std::vector<I> _addrs;
  std::hash<I> hash_fn;
};

}

#endif
