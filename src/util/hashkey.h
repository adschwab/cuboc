#ifndef HASHKEY_H
#define HASHKEY_H

#include <functional>

#include <glm/glm.hpp>

#include "util/hashalg.h"

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

  XYCoord(glm::vec3 vec) {
    this->setX(vec[0]);
    this->setY(vec[2]);
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

  bool operator!=(const XYCoord &other) const{
    return !(*this == other);
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

struct XYZCoord {
  
  int coords[3];

  XYZCoord() {
    this->setX(0);
    this->setY(0);
    this->setZ(0);
  }

  XYZCoord(int x_, int y_, int z_) {
    this->setX(x_);
    this->setY(y_);
    this->setZ(z_);
  }

  int x() const {
    return coords[0];
  }

  int y() const {
    return coords[1];
  }
  
  int z() const {
    return coords[2];
  }

  void setX(int x) {
    coords[0] = x;
  }

  void setY(int y) {
    coords[1] = y;
  }
  
  void setZ(int z) {
    coords[2] = z;
  }

  bool operator==(const XYZCoord &other) const{
    return (this->x() == other.x()
        && this->y() == other.y()
        && this->z() == other.z());
  }

  bool operator!=(const XYZCoord &other) const{
    return !(*this == other);
  }

};

template<>
struct std::hash<XYZCoord> {
  std::size_t operator()(const XYZCoord& k) const {
    return hashalg::hash((void *)k.coords, 12);
  }
};


#endif
