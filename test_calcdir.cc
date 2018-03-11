
#include "src/util/math_util.h"

#include <cstdio>

#include <glm/glm.hpp>

#define PI 3.14159f
int main() {

  glm::vec3 pos(1.0f,1.0f, 0.0f);
  
  glm::vec3 dir = cuboc::calcDir(PI/4.0f, 0.0f);;
  
  float dist = cuboc::distPlane(
      0, 0.0f, pos, dir);

  std::printf("POS: %f, %f, %f\n", pos[0], pos[1], pos[2]);
  std::printf("DIR: %f, %f, %f\n", dir[0], dir[1], dir[2]);
  std::printf("DIST: %f\n", dist);
  return 0;
}
