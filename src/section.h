#ifndef SECTION_H
#define SECTION_H

#include <unordered_map>
#include <vector>

#define BLOCK_SIZE 8

struct Index {
  
  int x;
  int y;
  int z;

  Index(int x_, int y_, int z_) : x(x_), y(y_), z(z_){}

  bool operator==(const Index &other) const{
    return (x == other.x
        && y == other.y
        && z == other.z);
  }

  Index& operator+=(const Index &other) {
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    return *this;
  }
};

inline Index operator+(Index lhs, const Index& rhs)
{
  lhs += rhs;
  return lhs;
}

template<>
struct std::hash<Index> {
  std::size_t operator()(const Index& k) const {
    return ((std::hash<int>()(k.x)
          ^ (std::hash<int>()(k.y) << 1)) >> 1
          ^ std::hash<int>()(k.z) << 1);
  }
};

class Section {

  public:
  Section(int size, Index index) : 
      _size(size), _index(index) {}

  virtual void set(
      int x1, int x2,
      int y1, int y2,
      int z1, int z2,
      Object &obj) = 0;

  virtual Object *get(int x, int y, int z) = 0;

  Object *get(Index idx) {
    return get(idx.x, idx.y, idx.z);
  }

  virtual void draw() = 0;

  virtual bool drawable() = 0;

  inline Index getIndex() {
    return _index;
  }
  
  inline int getSize() {
    return _size;
  }

  protected:
  int _size;
  Index _index;

  static inline Index getIndexAdjustment(int i) {
    return {i & 1, i & 2, i & 4};
  }
};

#endif
