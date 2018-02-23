#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <mutex>
#include <condition_variable>

#include "util/store.h"

#define DEFAULT_SIZE 211

namespace util {

template<typename I, typename D>
class Cache : public Store<I, D>{
  public:
  Cache(int size=DEFAULT_SIZE) :
      _size(size) {
    _cache = std::vector<D>(size);
    _addrs = std::vector<I>(size);
  }

  virtual bool contains(I index) {
    std::lock_guard<std::mutex> lk(_mtx);
    return index == _addrs[hash_fn(index) % _size];
  }

  virtual D *get(I index) {
    std::lock_guard<std::mutex> lk(_mtx);  
    if (index != _addrs[hash_fn(index) % _size])
      return nullptr;
    return &_cache[hash_fn(index) % _size];
  }

  virtual void add(I index, D data) {
    std::lock_guard<std::mutex> lk(_mtx);
    _addrs[hash_fn(index) % _size] = index;
    _cache[hash_fn(index) % _size] = data;
  }

  private:
  int _size;
  std::vector<D> _cache;
  std::vector<I> _addrs;
  std::hash<I> hash_fn;
  std::mutex _mtx;
};

}

#endif
