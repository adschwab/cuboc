#include "movement.h"

#include <cmath>
#include <cstdio>

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "world.h"
#include "util/macro.h"
#include "util/section_util.h"
#include "util/math_util.h"
#include "util/log.h"

#define MIN_DIST 0.0001f

namespace cuboc {

Movement::Movement(Camera *camera) :
    cam(camera),
    _left(false),
    _right(false),
    _forward(false),
    _backward(false),
    speed(2.0f) {
  last_step = glfwGetTime();
}   
 

void Movement::updatePosition(util::Store<XYZCoord, std::shared_ptr<BaseSection<Block> > > *world) {
  float time = glfwGetTime();
  float diff = time - last_step;
  last_step = time;

  if (!(_left || _right || _forward || _backward)) return;
  float left =_left;
  float right = _right;
  float forward = _forward;
  float backward = _backward;

  if ((left && forward) || (left && backward) ||
      (right && forward) || (right && backward)) {
    left *= 1.0f/glm::sqrt(2);
    right *= 1.0f/glm::sqrt(2);
    forward *= 1.0f/glm::sqrt(2);
    backward *= 1.0f/glm::sqrt(2);
  }
  
  cuboc::Position pos = cam->getPosition();
  
  float xdiff = 0.0f;
  float ydiff = 0.0f;
  if (_left) {
    ydiff += -speed * diff * left * glm::sin(cam->getXY());
    xdiff += speed * diff * left * glm::cos(cam->getXY());
  }
  if (_right) {
    ydiff += speed * diff * right * glm::sin(cam->getXY());
    xdiff += -speed * diff * right * glm::cos(cam->getXY());
  }
  if (_forward) {
    ydiff += speed * diff * forward * glm::cos(cam->getXY());
    xdiff += speed * diff * forward * glm::sin(cam->getXY());
  }
  if (_backward) {
    ydiff += -speed * diff * backward * glm::cos(cam->getXY());
    xdiff += -speed * diff * backward * glm::sin(cam->getXY());
  }

  check(
      pos.section,
      pos.offset,
      xdiff,
      ydiff,
      world);


  float new_x = pos.offset[0] + xdiff;
  float new_y = pos.offset[2] + ydiff;

  if (new_x > World::section_edge) {
    new_x -= World::section_edge;
    pos.section.setX(pos.section.x() + 1);
  }
  else if (new_x < 0.0f) {
    new_x += World::section_edge;
    pos.section.setX(pos.section.x() - 1);
  }
  // Currently not needed - z adjustments
  /*if (pos.offset[1] > World::section_edge) {
    pos.offset[1] -= World::section_edge;
    pos.section.setZ(pos.section.z() + 1);
  }
  else if (pos.offset[1] < 0.0f) {
    pos.offset[1] += World::section_edge;
    pos.section.setZ(pos.section.z() - 1);
  }*/
  if (new_y > World::section_edge) {
    new_y -= World::section_edge;
    pos.section.setY(pos.section.y() + 1);
  }
  else if (new_y < 0.0f) {
    new_y += World::section_edge;
    pos.section.setY(pos.section.y() - 1);
  }
  //std::printf("%d,%d,%d\n", pos.section.x(), pos.section.y(), pos.section.z());
  pos.offset[0] = new_x;
  pos.offset[2] = new_y;
  cam->setPosition(pos);
}

void Movement::check(
    XYZCoord section,
    glm::vec3 raw_pos,
    float &xdiff, float &ydiff,
    util::Store<XYZCoord, std::shared_ptr<BaseSection<Block> > > *store) {
 
  Block block;
  glm::vec3 pos(
      raw_pos[0],
      raw_pos[2],
      raw_pos[1]);
  glm::vec3 dir = calcDirXY(xdiff, ydiff);
  


  std::vector<std::array<int, 3> > offsets;
  float dist = std::sqrt(SQUARE(xdiff) + SQUARE(ydiff));
  walkLine(pos, dir, dist, World::block_size, offsets);
  //LOGF("x: %.2f, y: %.2f -> (%.2f, %.2f, %.2f)", xdiff, ydiff, dir[0], dir[1], dir[2]);
  
  for (int i = 0; i < offsets.size(); i ++) {
    std::array<int, 3> offset = offsets[i];

    bool found = get_block(
        store,
        section,
        World::section_count,
        offset,
        block);
    if (found && block.getType() != BLOCK_AIR) {

      float xnear = World::getSubOffset(offset[0]);
      float ynear = World::getSubOffset(offset[1]);
      if (std::abs(xnear - pos[0]) < MIN_DIST && xdiff > 0.0f) {
        xdiff = 0.0f;
        return;
      }
      else if (std::abs(xnear + World::block_size - pos[0]) < MIN_DIST && xdiff < 0.0f) {
        xdiff = 0.0f;
        return;
      }
      if (std::abs(ynear - pos[1]) < MIN_DIST && ydiff > 0.0f) {
        ydiff = 0.0f;
        return;
      }
      else if (std::abs(ynear + World::block_size - pos[1]) < MIN_DIST && ydiff < 0.0f) {
        ydiff = 0.0f;
        return;
      }
      float distf = MAX_DIST;

      if (xdiff > 0.0f) {
        distf = std::min(distf, distPlane(true, 0, xnear, pos, dir));
      }
      else if (xdiff < 0.0f) {
        distf = std::min(distf, distPlane(false, 0, xnear + World::block_size, pos, dir));

      }
      if (ydiff > 0.0f) {
        distf = std::min(distf, distPlane(true, 1, ynear, pos, dir));
      }
      else if (ydiff < 0.0f) {
        distf = std::min(distf, distPlane(false, 1, ynear + World::block_size, pos, dir));
      }

      //LOGF("([%d, %d] - [%.2f, %.2f]) -> %.2f, %.2f: %.2f", section.x(), section.y(),  pos[0], pos[1], xdiff, ydiff, distf);
      
      //xdiff = distf * xdiff / dist;
      //ydiff = distf * ydiff / dist;

      
      break;
    }
  }

}

} // namespace
