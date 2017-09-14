#include "movement.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

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
  
  if (left + right + forward + backward < 3 &&
      left + right + forward + backward > 1) {
    left *= 1.0f/glm::sqrt(2);
    right *= 1.0f/glm::sqrt(2);
    forward *= 1.0f/glm::sqrt(2);
    backward *= 1.0f/glm::sqrt(2);
  }
  
  glm::vec3 pos = cam->getPosition();
  if (left > 0.0f) {
    pos[2] -= speed * diff * left * glm::sin(cam->getXY());
    pos[0] += speed * diff * left * glm::cos(cam->getXY());
  }
  if (right > 0.0f) {
    pos[2] += speed * diff * right * glm::sin(cam->getXY());
    pos[0] -= speed * diff * right * glm::cos(cam->getXY());
  }
  if (forward > 0.0f) {
    pos[2] += speed * diff * forward * glm::cos(cam->getXY());
    pos[0] += speed * diff * forward * glm::sin(cam->getXY());
  }
  if (backward > 0.0f) {
    pos[2] -= speed * diff * backward * glm::cos(cam->getXY());
    pos[0] -= speed * diff * backward * glm::sin(cam->getXY());
  }
  cam->setPosition(pos);

  
}
