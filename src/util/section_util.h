#ifndef SECTION_UTIL_H
#define SECTION_UTIL_H

#include <cstdio>
#include <string>

#include "util/store.h"
#include "util/hashalg.h"
#include "util/section.h"
#include "util/log.h"

namespace cuboc {

bool get_block(
    util::Store<XYZCoord, std::shared_ptr<BaseSection<Block> > > *store,
    XYZCoord section,
    int edge,
    int xoff,
    int yoff,
    int zoff,
    Block &block) {
  section.setX(section.x() + xoff / edge);
  xoff = xoff % edge;
  section.setY(section.y() + yoff / edge);
  yoff = yoff % edge;
  section.setZ(section.z() + zoff / edge);
  zoff = zoff % edge;

  //LOGF("%s:%d, %d, %d", ((std::string)section).c_str(), xoff, yoff, zoff);

  if (!store->contains(section)) return false;
  BaseSection<Block> *s = store->get(section)->get();
  block = s->get(xoff, yoff, zoff);

  return true;
}

}

#endif
