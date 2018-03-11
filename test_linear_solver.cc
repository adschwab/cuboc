
#include "src/util/math_util.h"

#include <cstdio>

#include <glm/glm.hpp>

#define PI 3.14159f
int main() {

  glm::mat3 matrix = {5.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f, 3.0f, 0.0f, 1.0f};;

  std::printf("%.2f\n", matrix[0][1]);
  glm::vec3 d = {2.0f, -4.0f, 0.0f};

  glm::vec3 x = cuboc::solve(matrix, d);

  std::printf("%f, %f, %f\n", x[0], x[1], x[2]);
  return 0;
}
