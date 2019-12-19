#ifndef SECTION_UTIL_H
#define SECTION_UTIL_H

#include <cstdio>
#include <string>
#include <array>

#include "util/store.h"
#include "util/hashalg.h"
#include "util/section.h"
#include "util/log.h"

namespace cuboc {

bool get_block(
    util::Store<XYZCoord, std::shared_ptr<BaseSection<Block> > > *store,
    XYZCoord section,
    int edge,
    std::array<int, 3> offset,
    Block &block) {
  section.setX(section.x() + offset[0] / edge);
  offset[0] = (edge + offset[0] % edge) % edge;
  section.setY(section.y() + offset[1] / edge);
  offset[1] = (edge + offset[1] % edge) % edge;
  section.setZ(section.z() + offset[2] / edge);
  offset[2] = (edge + offset[2] % edge) % edge;

  if (!store->contains(section)) return false;
  BaseSection<Block> *s = store->get(section)->get();
  block = s->get(offset[0], offset[1], offset[2]);

  return true;
}

bool get_block_raw(
    util::Store<XYZCoord, std::shared_ptr<BaseSection<Block> > > *store,
    XYZCoord section,
    int edge, float block_size,
    float xraw, float yraw, float zraw,
    Block &block) {

  std::array<int, 3> offset;
  offset[0] = xraw / block_size;
  offset[1] = yraw / block_size;
  offset[2] = zraw / block_size;

  return get_block(store, section, edge, offset, block);
}

}

#endif
