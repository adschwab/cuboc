#include "world.h"

#include <vector>

#include <glm/glm.hpp>

#include "loader/programloader.h"
#include "loader/texture_loader.h"
#include "block.h"

DrawableSection::DrawableSection(
    BaseSection<BaseBlock> *section) {
  if (section->isContiguous()) return;

  std::vector<float> vertices;
  std::vector<unsigned char> indices;

  for (int i = 0; i < section.get_edge(); i ++) {
    for (int j = 0; j < section.get_edge(); j ++) {
      for (int k = 0; k < section.get_edge(); k ++) {
        BaseBlock block = section.get(i, j, k);
        if (block.getType() == BLOCK_AIR) continue;
        
        float i_f = (float) i;
        float j_f = (float) j;
        float k_f = (float) k;
        vertices.push_back(i_f);
      }
    }
  }
}


World::World(
    graphicsutils::ProgramLoader *program,
    TextureFactory *atlas) :
    _program(program),
    _tex_atlas(atlas) {

  

}

