#include "camera.h"

#include <cstdio>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "window.h"

#define PI 3.1415927f


Camera::Camera(
    base::Window *window,
    glm::vec3 pos, 
    float xy_angle,
    float yz_angle,
    float sensitivity):
      _window(window),
      _pos(pos),
      _xy(xy_angle),
      _yz(yz_angle), 
      _root_forward(glm::vec3(0.0f, 0.0f, 1.0f)),
      _sensitivity(sensitivity) {
  update_view();
}

float Camera::getXY() {
  return _xy;
}

float Camera::getYZ() {
  return _yz;
}

glm::mat4 Camera::getProj() {
  return glm::perspective(
      glm::radians(45.0f),
      (float)_window->getWidth()/(float)_window->getHeight(),
      0.1f,
      100.0f); 
}

glm::mat4 Camera::getView() {
  return view;
}

glm::vec3 Camera::getPosition() {
  return _pos;
}

void Camera::setPosition(glm::vec3 new_pos) {
  _pos = new_pos;
  update_view();
}

void Camera::updateAngles(
    float xdiff,
    float ydiff) {
  _xy = _xy - xdiff * PI/(180.0f * _sensitivity);
  if (_xy > PI) _xy = _xy - 2.0f * PI;
  else if (_xy < -PI) _xy = _xy + 2.0f * PI;
  _yz = glm::max(glm::min(
      _yz - ydiff * PI/(180.0f * _sensitivity),
      PI/2.0f),-PI/2.0f);
  update_view();
}

void Camera::update_view() {
  view = glm::mat4();
  glm::vec3 pos = _pos;
  pos[1] = -pos[1];
  view = glm::translate(view, pos);
  float xang = glm::sin(-_xy);
  float yang = glm::cos(-_xy);
  glm::vec3 angle = glm::vec3(xang, 0.0f, yang);
  view = glm::toMat4(rotation(_root_forward, angle)) * view;

  yang = glm::cos(_yz);
  float zang = glm::sin(_yz);
  angle = glm::vec3(0.0f, zang, yang);
  view = glm::toMat4(rotation(
      _root_forward, angle)) * view;
}

glm::quat Camera::rotation(glm::vec3 start, glm::vec3 dest) {

  float cosTheta = glm::dot(start, dest);
  glm::vec3 rotationAxis;
  if (cosTheta < -1 + 0.00001f) {
    rotationAxis = glm::cross(
        glm::vec3(0.0f, 0.0f, 1.0f), start);
    if (length2(rotationAxis) < 0.00001 )
      rotationAxis = glm::cross(
          glm::vec3(1.0f, 0.0f, 0.0f), start);
    rotationAxis = glm::normalize(rotationAxis);
    return glm::angleAxis(
        glm::radians(180.0f), rotationAxis);
  }
  rotationAxis = glm::cross(start, dest);
  float scale = glm::sqrt( (1+cosTheta)*2 );
  float invs = 1 / scale;
  return glm::quat(
      scale * 0.5f,
      rotationAxis.x * invs,
      rotationAxis.y * invs,
      rotationAxis.z * invs);
}

