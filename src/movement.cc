#include "movement.h"

#include <cmath>
#include <cstdio>

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "world.h"
#include "util/macro.h"
#include "util/section_util.h"
#include "util/log.h"

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

  //int curr_x = (int)(pos.offset[0] * World::section_count);
  //int curr_y = (int)(pos.offset[2] * World::section_count);

  check(
      pos.section,
      pos.offset,
      xdiff,
      ydiff,
      world);


  //LOGF("%d, %d", (int)(pos.offset[0] * World::section_count), (int)(pos.offset[2] * World::section_count));
  
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
    glm::vec3 offset,
    float &xdiff, float &ydiff,
    util::Store<XYZCoord, std::shared_ptr<BaseSection<Block> > > *store) {
 
  Block block;
  float max_dist = cam->getMaxDist();

  int x_ind = World::getSubIndex(offset[0]);
  int y_ind = World::getSubIndex(offset[2]);
  int z_ind = World::getSubIndex(offset[1]);
  if (xdiff > 0.0f) {
    for (int i = World::getSubIndex(offset[0] + max_dist); i <= World::getSubIndex(offset[0] + xdiff + max_dist); i ++) {
      bool exists = cuboc::get_block(
          store,
          section,
          World::section_count,
          i, y_ind, z_ind,
          block);
      if (exists && block.getType() != 0) {
        xdiff = -offset[0] + World::getSubOffset(i) - max_dist;
        break;
      }
    }
  }
  if (ydiff > 0.0f) {
    for (int i = World::getSubIndex(offset[2] + max_dist); i <= World::getSubIndex(offset[2] + ydiff + max_dist); i ++) {
      bool exists = cuboc::get_block(
          store,
          section,
          World::section_count,
          x_ind, i, z_ind,
          block);
      if (exists && block.getType() != 0) {
        ydiff = -offset[2] + World::getSubOffset(i) - max_dist;
        break;
      }
    }
  }
}

} // namespace
