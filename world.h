#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include <vector>

#include "loader/object.h"

#define BLOCK_SIZE 8
#define SECTION_SIZE 2

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
};

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
  Section(int size) : 
      _size(size) {}
  virtual ~Section() {};


  virtual void set(
      int x1, int x2,
      int y1, int y2,
      int z1, int z2,
      Object &obj) = 0;

  virtual Object *get(int x, int y, int z) = 0;

  virtual void draw() = 0;

  private:
  int _size;

};

class ObjectSection : public Section {
  
  public:
  ObjectSection(int size, Object *object) :
      Section(size),
      _obj(object) {}

  Object *get(int x, int y, int z) {
    return _obj;
  }
  
  void set(
      int x1, int x2,
      int y1, int y2,
      int z1, int z2,
      Object &obj);

  void draw();

  private:
  Object *_obj;

};

class SubSection : public Section {

  public:
  SubSection(int size) : 
      Section(size) {}

  virtual ~SubSection() {
    sections.clear();
  }

  virtual void set(
      int x1, int x2,
      int y1, int y2,
      int z1, int z2,
      Object &obj);

  virtual Object *get(int x, int y, int z);

  virtual void draw();

  private:
  std::unordered_map<Index, Section*> sections;
};

class World {
  
  public:
  World() {}
  
  ~World() {
    sections.clear();
  }
  
  void set(
      int x1, int x2,
      int y1, int y2,
      int z1, int z2,
      Object &obj);

  Object *get(int x, int y, int z);

  void draw(int x, int y, int z, float radius);

  private:
  std::unordered_map<Index, Section*> sections;
};

#endif
