
#include "src/util/math_util.h"

#include <cstdio>

#include <glm/glm.hpp>

#define PI 3.14159f
int main() {

  glm::vec3 pos(1.0f,0.0f, 0.0f);
  
  glm::vec3 dir = cuboc::calcDirXY(1.0f, 0.0f);;
  
  std::vector<std::array<int, 3> > offsets;
  cuboc::walkLine(pos, dir, 1.5f, 0.5f, offsets);


  std::printf("DIR: %f, %f, %f\n", dir[0], dir[1], dir[2]);
  for (std::array<int, 3> coord : offsets) {
    std::printf("%d, %d, %d\n", coord[0], coord[1], coord[2]);
  }
  return 0;
}
