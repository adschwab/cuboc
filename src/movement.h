#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "block.h"
#include "util/store.h"
#include "util/section.h"

namespace cuboc {

class Movement {

  public:
  Movement(Camera *camera);

  void setLeft(bool left) {
    _left = left;
  }

  void setRight(bool right) {
    _right = right;
  }

  void setForward(bool forward) {
    _forward = forward;
  }

  void setBackward(bool backward) {
    _backward = backward;
  }
  
  void updatePosition(util::Store<XYZCoord, std::shared_ptr<BaseSection<Block> > > *world);

  private:
  void check(
      XYZCoord section,
      glm::vec3 offset,
      float &xdiff, float &ydiff,
      util::Store<XYZCoord, std::shared_ptr<BaseSection<Block> > > *world);
  
  Camera *cam;
  float speed;
  
  bool _left;
  bool _right;
  bool _forward;
  bool _backward;

  float last_step;
};

} // namespace
#endif
