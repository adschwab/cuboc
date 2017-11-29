#include "world.h"

#include <cstdio>
#include <cstdlib>
#include <memory>

#include "loader/object.h"
#include "util/math_util.h"

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

        Index idx = {i, j, k};
        int cx = i << BLOCK_SIZE;
        int cy = j << BLOCK_SIZE;
        int cz = k << BLOCK_SIZE;

        int bx1, bx2, by1, by2, bz1, bz2;
        int size = 1 << BLOCK_SIZE;
        std::printf("%d\n", size);
        bool ok = findBounds(x1, x2,
            y1, y2,
            z1, z2,
            cx, cy, cz, size, 
            bx1, bx2, 
            by1, by2,
            bz1, bz2);
        if (!ok) {
          std::printf("Out of bounds\n");
          continue;
        }

        if (bx2 - bx1 +
            by2 - by1 +
            bz2 - bz1 == size * 3) {
          std::shared_ptr<Section> section = 
            std::make_shared<ObjectSection>(
              size, idx, &obj);
          sections.insert({idx, section});
          continue;
        }
        if (sections.find(idx) == sections.end()) {
          
          std::shared_ptr<Section> section =
            std::make_shared<SubSection>(
              BLOCK_SIZE, idx);
          section->set(
              bx1, bx2,
              by1, by2,
              bz1, bz2, obj);
          sections.insert({idx, section});
          continue;
        }
        
        auto item = sections.find(idx);
        
        if (item->second->drawable()){
          
          std::shared_ptr<Section> section =
            std::make_shared<SubSection>(
              BLOCK_SIZE, idx, item->second->get(idx));
          section->set(
              bx1, bx2,
              by1, by2,
              bz1, bz2, obj);
          sections.insert({idx, section});
        }
        else {
          item->second->set(
              bx1, bx2,
              by1, by2,
              bz1, bz2, obj);
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


SubSection::SubSection(int size, Index index) :
    Section(size, index) {
  if (size == 1) {
    // TODO Error
    std::printf("Error: cannot instantiate a size 1 subsection\n");
    std::exit(1);
  }
}

SubSection::SubSection(int size, Index index, Object *obj) :
    SubSection(size, index) {

  for (int i = 0; i < 8; ++i) {
    Index change = Section::getIndexAdjustment(i);
    
    subsections[i] = std::unique_ptr<Section>( 
        new ObjectSection(
          size >> 1,
          index + change,
          obj));
  }
}

void SubSection::set(
    int x1, int x2,
    int y1, int y2,
    int z1, int z2,
    Object &obj) {
  if (x1 < _index.x)
    x1 = _index.x;
  if (y1 < _index.y)
    y1 = _index.y;
  if (z1 < _index.z)
    z1 = _index.z;
  if (x2 >= _index.x + _size)
    x2 = _index.x + _size - 1;
  if (y2 >= _index.y + _size)
    y2 = _index.y + _size - 1;
  if (z2 >= _index.z + _size)
    z2 = _index.z + _size - 1;


}

Object *SubSection::get(int x, int y, int z) {

  return nullptr;
}

void SubSection::draw() {

}

