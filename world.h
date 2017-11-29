#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include <vector>
#include <memory>

#include "loader/object.h"
#include "section.h"


class ObjectSection : public Section {
  
  public:
  ObjectSection(int size, Index index, Object *object) :
      Section(size, index),
      _obj(object) {}

  Object *get(int x, int y, int z) {
    return _obj;
  }
  
  void set(
      int x1, int x2,
      int y1, int y2,
      int z1, int z2,
      Object &obj);

  virtual void draw();

  virtual bool drawable() { return true; };
  private:
  Object *_obj;
};

class SubSection : public Section {

  public:
  SubSection(int size, Index index);

  SubSection(int size, Index index, Object *obj);

  virtual void set(
      int x1, int x2,
      int y1, int y2,
      int z1, int z2,
      Object &obj);

  virtual Object *get(int x, int y, int z);

  virtual void draw();

  virtual bool drawable() { return false; }
  
  private:
  std::unique_ptr<Section> subsections[8];
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
  std::unordered_map<Index, std::shared_ptr<Section> > sections;
};

#endif
