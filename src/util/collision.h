#ifndef UTIL_COLLISION_H
#define UTIL_COLLISION_H

#include <vector>
#include <array>

#include <glm/glm.hpp>

#include "util/log.h"

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

    is_block[0] = func(offset[0], offset[1], offset[2]);
    is_block[1] = func(offset[0] + signs[0], offset[1], offset[2]);
    is_block[2] = func(offset[0], offset[1] + signs[1], offset[2]);
    is_block[3] = func(offset[0], offset[1], offset[2] + signs[2]);
    is_block[4] = func(offset[0] + signs[0], offset[1] + signs[1], offset[2]);
    is_block[5] = func(offset[0], offset[1] + signs[1], offset[2] + signs[2]);
    is_block[6] = func(offset[0] + signs[0], offset[1], offset[2] + signs[2]);
    is_block[7] = func(offset[0] + signs[0], offset[1] + signs[1], offset[2] + signs[2]);
i
    for (int i = 0; i < 8; i ++) {
      float close = radius;
      if (is_block[i]) {
        if (i == 1 || i == 2 || i == 3) {
          float distToPlane = distPlane(
              signs[i - 1] > 0,
              i - 1,
              (offset[i - 1] + signs[i - 1]) * size - signs[i - 1] * radius,
              pos,
              dir);
          if (distToPlane < close)
            close = distToPlane;

        }
        else if (i == 4 || i == 5 || i == 6) {
          // TODO: edge detection
        }
        else if (i == 7) {
          // TODO: point detection
        }
      }
      if (close < dist)
        dist = close;

    }
  }
  return dist;
}

}

#endif
