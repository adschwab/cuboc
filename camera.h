#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Camera {
  
  public:
  Camera(glm::vec3 pos, float xy_angle, float yz_angle);
  
  glm::mat4 getView();
  glm::vec3 getPosition();
  
  float getXY();

  float getYZ();
  
  void setPosition(glm::vec3 new_pos);
  
  void updateAngles(
      float xdiff,
      float ydiff,
      float sensitivity);

  void update_view();

  private:

  glm::quat rotation(glm::vec3 start, glm::vec3 dest);

  glm::mat4 view;
  glm::vec3 _root_forward;
  glm::vec3 _pos;
  float _xy;
  float _yz;

};

#endif
