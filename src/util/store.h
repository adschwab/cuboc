#ifndef UTIL_STORE_H
#define UTIL_STORE_H

namespace util {

template<typename I, typename D>
class Store{
  public:
  virtual bool contains(I index) = 0;

  virtual D *get(I index) = 0;

  virtual void add(I index, D data) = 0;
};

}
#endif
