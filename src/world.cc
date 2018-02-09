#include "world.h"

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace cuboc {


World::World(
    graphicsutils::ProgramLoader *program,
    TextureLoader *atlas) :
    _program(program),
    _tex_atlas(atlas) {

  _raw = std::make_shared<util::Cache<XYZCoord, std::shared_ptr<BaseSection<Block> > > >();
  _draw_cache = std::make_shared<util::Cache<XYZCoord, DrawableSection > > ();

  std::shared_ptr<Section<Block> > test_section = std::make_shared<Section<Block> > ();
  test_section->set(Block(BLOCK_BOX), 0, 0, 0);
  _raw->add(XYZCoord(), test_section);
  DrawableSection drawable(test_section.get(), BLOCK_SIZE);
  _draw_cache->add(XYZCoord(), drawable);
}

void World::draw(XYZCoord coord_pos, glm::vec3 offset) {
  _program->use();
  _tex_atlas->set(_program);

  glm::mat4 model;
  offset[1] = -offset[1];
  model = glm::translate(model, -offset);
  _program->setMatrix("model", model);

  DrawableSection drawable = _draw_cache->get(XYZCoord());
  
  drawable.draw(offset);
}

}


