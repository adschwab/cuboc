
#include <cstdio>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "src/util/math_util.h"
#include "src/util/collision.h"

struct printer {

  bool operator()(int x, int y, int z) {
    //std::printf("%d,%d,%d\n", x, y, z);
    
    if (x == 1 && y == 1 && z == 0)
      return true;
    return false;
  }
};

int main() {

  glm::vec3 pos(0.4f, 0.35f, 0.1f);
  glm::vec3 dir(1.0f, 1.0f, 0.0f);
  dir = glm::normalize(dir);

  float dist = 0.2f;
  float size = 0.5;
  float radius = 0.14f;

  float new_dist = cuboc::collide(pos, dir, dist, size, radius, printer());
  glm::vec3 diffs;
  cuboc::calcDiffs(dir, new_dist, diffs);
  pos[0] += diffs[0];
  pos[1] += diffs[1];
  pos[2] += diffs[2];  
  std::printf("%.2f -> (%.2f, %.2f, %.2f)\n", new_dist, pos[0], pos[1], pos[2]);
  return 0;
}
