#include "movement.h"

#include "util/section.h"
#include "world.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace cuboc {

void Movement::updatePosition() {
  float time = glfwGetTime();
  float diff = time - last_step;
  last_step = time;

  float left =_left;
  float right = _right;
  float forward = _forward;
  float backward = _backward;

  if (_left && _right) {
    left = 0.0f;
    right = 0.0f;
  }
  if (_forward && _backward) {
    forward = 0.0f;
    backward = 0.0f;
  }
  
  if (left + right + forward + backward < 2.5 &&
      left + right + forward + backward > 1.5) {
    left *= 1.0f/glm::sqrt(2);
    right *= 1.0f/glm::sqrt(2);
    forward *= 1.0f/glm::sqrt(2);
    backward *= 1.0f/glm::sqrt(2);
  }
  
  cuboc::Position pos = cam->getPosition();
  if (left > 0.01f) {
    pos.offset[2] -= speed * diff * left * glm::sin(cam->getXY());
    pos.offset[0] += speed * diff * left * glm::cos(cam->getXY());
  }
  if (right > 0.01f) {
    pos.offset[2] += speed * diff * right * glm::sin(cam->getXY());
    pos.offset[0] -= speed * diff * right * glm::cos(cam->getXY());
  }
  if (forward > 0.01f) {
    pos.offset[2] += speed * diff * forward * glm::cos(cam->getXY());
    pos.offset[0] += speed * diff * forward * glm::sin(cam->getXY());
  }
  if (backward > 0.01f) {
    pos.offset[2] -= speed * diff * backward * glm::cos(cam->getXY());
    pos.offset[0] -= speed * diff * backward * glm::sin(cam->getXY());
  }
  cam->setPosition(pos);
  if (pos.offset[0] > World::section_edge) {
    pos.offset[0] -= World::section_edge;
    pos.section.setX(pos.section.x() + 1);
  }
  else if (pos.offset[0] < 0.0f) {
    pos.offset[0] += World::section_edge;
    pos.section.setX(pos.section.x() - 1);
  }
  if (pos.offset[1] > World::section_edge) {
    pos.offset[1] -= World::section_edge;
    pos.section.setZ(pos.section.z() + 1);
  }
  else if (pos.offset[1] < 0.0f) {
    pos.offset[1] += World::section_edge;
    pos.section.setZ(pos.section.z() - 1);
  }
  if (pos.offset[2] > World::section_edge) {
    pos.offset[2] -= World::section_edge;
    pos.section.setY(pos.section.y() + 1);
  }
  else if (pos.offset[2] < 0.0f) {
    pos.offset[2] += World::section_edge;
    pos.section.setY(pos.section.y() - 1);
  }
}

} // namespace
