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
  offset[0] = offset[0] % edge;
  section.setY(section.y() + offset[1] / edge);
  offset[1] = offset[1] % edge;
  section.setZ(section.z() + offset[2] / edge);
  offset[2] = offset[2] % edge;

  //LOGF("%s:%d, %d, %d", ((std::string)section).c_str(), xoff, yoff, zoff);

  if (!store->contains(section)) return false;
  BaseSection<Block> *s = store->get(section)->get();
  block = s->get(offset[0], offset[1], offset[2]);

  return true;
}

}

#endif
