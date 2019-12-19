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

  float zdiff = 0.0f;
  check(
      pos.section,
      pos.offset,
      xdiff,
      ydiff,
      zdiff,
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
    float &xdiff, float &ydiff, float &zdiff,
    util::Store<XYZCoord, std::shared_ptr<BaseSection<Block> > > *store) {
 
  Block block;
  std::array<int, 3> offset;
  float dist = std::sqrt(SQUARE(xdiff) + SQUARE(ydiff) + SQUARE(zdiff));

 if (dist > BLOCK_SIZE) {
    xdiff = xdiff * BLOCK_SIZE / dist;
    ydiff = ydiff * BLOCK_SIZE / dist;
    zdiff = zdiff * BLOCK_SIZE / dist;
  }
  float new_x = raw_pos[0] + xdiff;
  float new_y = raw_pos[2] + ydiff;
  float new_z = raw_pos[1] + zdiff;

  int xsgn = xdiff >= 0 ? 1 : -1;
  int ysgn = ydiff >= 0 ? 1 : -1;
  int zsgn = zdiff >= 0 ? 1 : -1;


  for (char i = 0; i < 8; i ++) {
    
    float x = new_x + xsgn * (i & 1);
    float y = new_y + ysgn * ((i >> 1) & 1);
    float z = new_z + zsgn * ((i >> 2) & 1);
    
    bool found = get_block_raw(
        store,
        section,
        World::section_count,
        BLOCK_SIZE,
        x, y, z,
        block);
    if (found && block.getType() != BLOCK_AIR) {
      xdiff = 0.0f;
      ydiff = 0.0f;
      zdiff = 0.0f;
    }
  }  
}

} // namespace
