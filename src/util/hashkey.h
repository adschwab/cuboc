#ifndef HASHKEY_H
#define HASHKEY_H

#include <functional>

#include <glm/glm.hpp>

struct XYCoord {
  
  union coords {
    float vals[2];
    double hash;
  } coords;

  XYCoord() {
    this->setX(0.0f);
    this->setY(0.0f);
  }

  XYCoord(float x_, float y_) {
    this->setX(x_);
    this->setY(y_);
  }

  float x() const {
    return coords.vals[0];
  }

  float y() const {
    return coords.vals[1];
  }

  void setX(float x) {
    coords.vals[0] = x;
  }

  void setY(float y) {
    coords.vals[1] = y;
  }

  bool operator==(const XYCoord &other) const{
    return (this->x() == other.x()
        && this->y() == other.y());
  }

  XYCoord& operator+=(const XYCoord &other) {
    this->setX(this->x() + other.x());
    this->setY(this->y() + other.y());
    return *this;
  }

  glm::vec3 vec() const {
    return glm::vec3(this->x(), 0.0f, this->y());
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
    return std::hash<double>()(k.coords.hash);
  }
};


#endif
