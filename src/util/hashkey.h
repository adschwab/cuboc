#ifndef HASHKEY_H
#define HASHKEY_H

#include <functional>

#include <glm/glm.hpp>

struct XYCoord {
  
  float x;
  float y;

  XYCoord(float x_, float y_) : x(x_), y(y_) {}

  bool operator==(const XYCoord &other) const{
    return (x == other.x
        && y == other.y);
  }

  XYCoord& operator+=(const XYCoord &other) {
    this->x += other.x;
    this->y += other.y;
    return *this;
  }

  glm::vec3 vec() const {
    return glm::vec3(x, 0.0f, y);
  }
};

inline XYCoord operator+(XYCoord lhs, const XYCoord& rhs)
{
  lhs += rhs;
  return lhs;
}

template<>
struct std::hash<XYCoord> {
  std::size_t operator()(const XYCoord& k) const {
    return std::hash<float>()(k.x) ^ (std::hash<float>()(k.y) << 1);
  }
};


#endif
