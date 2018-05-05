
#include "src/util/math_util.h"

#include <cstdio>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "src/util/line.h"
int main() {

  glm::vec3 v1 = {1.0f, 1.0f, 0.0f};
  glm::vec3 dir1_raw = {1.0f, 1.0f, 0.0f};
  glm::vec3 d1 = glm::normalize(dir1_raw);
  glm::vec3 v2 = {1.0f, 1.0f, 1.0f};
  glm::vec3 dir2_raw = {-1.0f, -1.0f, 0.0f};
  glm::vec3 d2 = glm::normalize(dir2_raw);

  cuboc::Line l = {v1, d1};
   
  std::printf("%.2f\n", l.distLine({v2, d2}));
  
  return 0;
}
