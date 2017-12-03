#include "gen/ground_gen.h"

#include <memory>
#include <random>

#include <GL/glew.h>

std::shared_ptr<std::vector<GLfloat>> gen_ground_data(
    int xsz,
    int ysz) {

  float mean = 0.0f;
  float sd = 0.2f;

  std::normal_distribution<float> dis(
      mean,
      sd);

  std::random_device rd;
  std::mt19937 gen(rd());
  
  std::shared_ptr<std::vector<GLfloat>> heights = 
      std::make_shared<std::vector<GLfloat>>();
  for (int i = 0; i < xsz; i ++) {
    for (int j = 0; j < ysz; j ++) {
      float change = dis(gen);
      heights->push_back(change);
    }
  }
  return heights;
}
