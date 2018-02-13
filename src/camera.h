#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "window.h"
#include "util/hashkey.h"

#define DEFAULT_SENSITIVITY 5

namespace cuboc {

struct Position {

  Position(glm::vec3 o, XYZCoord s) : offset(o), section(s) {}
  
  glm::vec3 offset;
  XYZCoord section;

};

class Camera {
  
  public:
  Camera(
      base::Window *window,
      glm::vec3 pos,
      float xy_angle,
      float yz_angle,
      float sensitivity=DEFAULT_SENSITIVITY);
  
  glm::mat4 getProj();
  glm::mat4 getView();
  Position getPosition();
  
  float getXY();

  float getYZ();
  
  void setPosition(Position new_pos);
  
  void updateAngles(
      float xdiff,
      float ydiff);

  void update_view();

  private:

  glm::quat rotation(glm::vec3 start, glm::vec3 dest);
  base::Window *_window;
  glm::mat4 view;
  glm::vec3 _root_forward;
  glm::vec3 _pos;
  XYZCoord _section;
  float _xy;
  float _yz;
  float _sensitivity;
};

} // namespace

#endif
