#include "gen/ground_gen.h"

#include <memory>
#include <random>

#include <GL/glew.h>

void gen_ground_data(
    std::unordered_map<XYCoord, Block> &blocks,
    std::shared_ptr<std::vector<GLfloat> > out_heights,
    std::shared_ptr<std::vector<int> > out_tex_ids) {
  float mean = 0.0f;
  float sd = 0.2f;


  std::normal_distribution<float> dis(
      mean,
      sd);

  std::random_device rd;
  std::mt19937 gen(rd());
  
  for (int i = 0; i < BLOCK_SIZE; i ++) {
    for (int j = 0; j < BLOCK_SIZE; j ++) {
      float change = dis(gen);
      out_heights->push_back(change);
    }
  }

  for (int i = 0; i < 4; ++i) {
    out_tex_ids->push_back(dis(gen) > 0 ? 0 : 1);
  }
}
