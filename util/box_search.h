#ifndef BOX_SEARCH_H
#define BOX_SEARCH_H

#include <unordered_map>

#include "section.h"

#define PCT_HIT 0.01 // This should be 1/sqrt(num blocks)

inline bool loc_in_box(Index loc, Index box, int size) {

  return loc.x >= box.x && loc.x < box.x + size 
      && loc.y >= box.y && loc.y < box.y + size 
      && loc.z >= box.z && loc.z < box.z + size;
}

Section *box_search(
    std::unordered_map<Index, Section*> &sections,
    Index loc,
    Index &corner) {

  // Is the number of sections relatively small
  if (sections.size() < (1 << (BLOCK_SIZE * 3)) * PCT_HIT) {
    for (auto iter = sections.begin(); iter != sections.end(); iter ++) {
      Index idx = iter->first;
      int size = iter->second->getSize();
      if (loc_in_box(loc, idx, size)) return iter->second;
    }
    return NULL;
  }

  // We know there are many sections, shouldn't take long
  Index idx = {loc.x, loc.y, loc.z};
  while (idx.x >= corner.x 
      && idx.y >= corner.y 
      && idx.z >= corner.z) {
    auto elem = sections.find(idx);
    if (elem != sections.end()) {
      if (loc_in_box(loc, elem->first, iter->second->getSize()))
        return iter->second;
      if (idx.y > corner.y)
        idx.y -= 1;
      else {
        idx.y = loc.y
        idx.z -= 1;
      }
      idx.x = loc.x;
    }
    else {
      if (idx.x > corner.x)
        idx.x -= 1;
      else if (idx.y > corner.y) {
        idx.x = loc.x;
        idx.y -= 1;
      }
      else {
        idx.x = loc.x;
        idx.y = loc.y;
        idx.z -= 1;
      }
    }
  }
  return NULL;
}


#endif
