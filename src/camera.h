#ifndef CAMERA_H
#define CAMERA_H

#include <mutex>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "window.h"
#include "util/hashkey.h"

// fundamental camera stats - assume cam_close << block_size
#define DEFAULT_SENSITIVITY 5
#define DEFAULT_CAM_CLOSE 0.1f
#define DEFAULT_WIDTH_ANG glm::radians(45.0f)

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
      glm::vec3 pos_init,
      float xy_angle,
      float yz_angle,
      float sensitivity=DEFAULT_SENSITIVITY,
      const float cam_close=DEFAULT_CAM_CLOSE,
      const float width_ang=DEFAULT_WIDTH_ANG);
  
  glm::mat4 getProj();
  glm::mat4 getView();
  Position getPosition();
  
  float getXY();

  float getYZ();
  
  base::Window *getWindow() { return _window; }

  void setPosition(Position new_pos);
  
  void updateAngles(
      float xdiff,
      float ydiff);

  void update_view();

  float getMaxDist();

  float widthOver2();

  float heightOver2();

  private:
  void updateCache();

  glm::quat rotation(glm::vec3 start, glm::vec3 dest);
  base::Window *_window;
  glm::mat4 view;
  glm::vec3 _root_forward;
  glm::vec3 _pos;
  XYZCoord _section;
  float _xy;
  float _yz;
  float _sensitivity;

  const float _cam_close;
  const float _width_ang;
  float _max_dist;
  int win_width;
  int win_height;
  float _cam_width;
  float _cam_height;

  std::mutex _mtx;
};

} // namespace

#endif
