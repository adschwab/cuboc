#include "world.h"

#include <cstdio>

#include "loader/object.h"

void World::set(
    int x1, int x2,
    int y1, int y2,
    int z1, int z2,
    Object &obj) {
  // We will for now assume x1, y1, z1 are lower
  Index idx1(x1 >> BLOCK_SIZE,
      y1 >> BLOCK_SIZE,
      z1 >> BLOCK_SIZE);

  Index idx2(x2 >> BLOCK_SIZE,
      y2 >> BLOCK_SIZE,
      z2 >> BLOCK_SIZE);;

  for (int i = idx1.x; i <= idx2.x; i ++) {
    for (int j = idx1.y; j <= idx2.y; j ++) {
      for (int k = idx1.z; k <= idx2.z; k ++) {
        Index idx(i, j, k);
        Section *section = NULL;
        auto found = sections.find(idx);
        if (found == sections.end()) {
          section = new SubSection(BLOCK_SIZE);
          sections.insert({idx, section});
        }
        else {
          section = found->second;
        }
        
      }
    }
  }
}

Object *World::get(int x, int y, int z) {

  return nullptr;
}

void World::draw(int x, int y, int z, float radius) {

}


void ObjectSection::set(
    int x1, int x2,
    int y1, int y2,
    int z1, int z2,
    Object &obj) {
  
}

void ObjectSection::draw() {

}


void SubSection::set(
    int x1, int x2,
    int y1, int y2,
    int z1, int z2,
    Object &obj) {
  
}

Object *SubSection::get(int x, int y, int z) {

  return nullptr;
}

void SubSection::draw() {

}

