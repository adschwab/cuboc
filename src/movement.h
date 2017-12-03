#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <GLFW/glfw3.h>

#include "camera.h"

class Movement {

  public:
  Movement(Camera *camera):
      cam(camera),
      _left(false),
      _right(false),
      _forward(false),
      _backward(false),
      speed(1.0f){
    
    last_step = glfwGetTime();
  }  

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
  
  void updatePosition(); 

  private:

  Camera *cam;
  float speed;
  
  bool _left;
  bool _right;
  bool _forward;
  bool _backward;

  float last_step;
};

#endif
