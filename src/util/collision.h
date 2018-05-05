#ifndef UTIL_COLLISION_H
#define UTIL_COLLISION_H

#include <vector>
#include <array>

#include <glm/glm.hpp>

#include "util/log.h"
#include "util/line.h"

namespace cuboc {


template <typename F>
float collide(
    const glm::vec3 pos,
    const glm::vec3 dir, // This is assumed to be normalized
    float dist, float size, 
    float radius,
    F func) {
  std::vector<std::array<int, 3> > offsets;
  walkLine(pos, dir, dist, size, offsets);

  std::array<char, 3> signs;
  for (int i = 0; i < 3; i ++) 
    signs[i] = dir[i] >= 0.0f ? 1 : -1; 

  std::array<bool, 8> is_block;

  
  for (std::array<int, 3> offset : offsets) {
    bool hit = false;
    LOGF("%d,%d,%d", offset[0], offset[1], offset[2]);
    is_block[0] = func(offset[0], offset[1], offset[2]);
    is_block[1] = func(offset[0] + signs[0], offset[1], offset[2]);
    is_block[2] = func(offset[0], offset[1] + signs[1], offset[2]);
    is_block[3] = func(offset[0], offset[1], offset[2] + signs[2]);
    is_block[4] = func(offset[0] + signs[0], offset[1] + signs[1], offset[2]);
    is_block[5] = func(offset[0], offset[1] + signs[1], offset[2] + signs[2]);
    is_block[6] = func(offset[0] + signs[0], offset[1], offset[2] + signs[2]);
    is_block[7] = func(offset[0] + signs[0], offset[1] + signs[1], offset[2] + signs[2]);


    for (int i = 0; i < 8; i ++) {
      float close = radius;
      if (is_block[i]) {
        LOGF("Found i: %d", i);
        if (i == 1 || i == 2 || i == 3) {

          float distToPlane = distPlane(
              signs[i - 1] > 0,
              i - 1,
              (offset[i - 1] + signs[i - 1]) * size
                  - signs[i - 1] * radius,
              pos,
              dir);
          if (distToPlane < close) {
            close = distToPlane;
            hit = true;
          }
        }
        else if (i == 4 || i == 5 || i == 6) {
          glm::vec3 ldir;
          glm::vec3 lpos;
          for (int k = 0; k < 3; k ++) {
            if (k == i - 4) {
              ldir[3 - k - 1] = 1.0f;
              lpos[3 - k - 1] = 0.0f;
            }
            else {
              ldir[3 - k - 1] = 0.0f;
              lpos[3 - k - 1] = 
                  (offset[i - 4] + signs[i - 4]) * size;
            }
          }
          LOGF("%.2f,%.2f,%.2f", ldir[0], ldir[1], ldir[2]);
          LOGF("%.2f,%.2f,%.2f", lpos[0], lpos[1], lpos[2]);
          Line line = {lpos, ldir};
          Line walk = {pos, dir};
          float min_dist = walk.distLine(line);
          LOGF("MIN DIST: %.2f\n", min_dist);
          if (min_dist < radius) {
            // We have a hit on a corner
            hit = true;
          }
        }
        else if (i == 7) {
          // TODO: point detection
        }
      }
      if (hit && close < dist)
        dist = close;

    }
    if (hit)
      break;
  }
  return dist;
}

}

#endif
