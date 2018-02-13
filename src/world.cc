#include "world.h"

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace cuboc {

float World::section_edge = (float) BaseSection<int>::get_edge() * BLOCK_SIZE;

World::World(
    graphicsutils::Program3d *program,
    TextureLoader *atlas,
    unsigned char render_dist) :
    _program(program),
    _tex_atlas(atlas),
    _render_dist(render_dist) {

  _raw = std::make_shared<util::Cache<XYZCoord, std::shared_ptr<BaseSection<Block> > > >();
  _draw_cache = std::make_shared<util::Cache<XYZCoord, DrawableSection > > ();

  std::shared_ptr<Section<Block> > test_section = std::make_shared<Section<Block> > ();
  test_section->set(Block(BLOCK_BOX), 0, 0, 0);
  test_section->set(Block(BLOCK_BOX), 0, 0, 1);
  test_section->set(Block(BLOCK_BOX), 0, 1, 0);
  test_section->set(Block(BLOCK_BOX), 1, 0, 0);
  test_section->set(Block(BLOCK_BOX), 1, 1, 0);
  test_section->set(Block(BLOCK_BOX), 1, 0, 1);
  test_section->set(Block(BLOCK_BOX), 0, 1, 1);
  test_section->set(Block(BLOCK_BOX), 1, 1, 1);

  _raw->add(XYZCoord(), test_section);
  DrawableSection drawable(test_section.get(), BLOCK_SIZE);
  _draw_cache->add(XYZCoord(), drawable);
  
  std::shared_ptr<Section<Block> > test_section2 = std::make_shared<Section<Block> > ();
  test_section2->set(Block(BLOCK_EARTH), 0, 0, 0);
  test_section2->set(Block(BLOCK_EARTH), 0, 0, 1);
  test_section2->set(Block(BLOCK_EARTH), 0, 1, 0);
  test_section2->set(Block(BLOCK_EARTH), 1, 0, 0);
  test_section2->set(Block(BLOCK_EARTH), 1, 1, 0);
  test_section2->set(Block(BLOCK_EARTH), 1, 0, 1);
  test_section2->set(Block(BLOCK_EARTH), 0, 1, 1);
  test_section2->set(Block(BLOCK_EARTH), 1, 1, 1);
  _raw->add(XYZCoord(1, 0, 0), test_section2);
  DrawableSection drawable2(test_section2.get(), BLOCK_SIZE);
  _draw_cache->add(XYZCoord(1, 0, 0), drawable2);
}

void World::draw() {
  Position pos = _program->camera()->getPosition();;
  XYZCoord coord_pos = pos.section;
  _program->use();
  _tex_atlas->set(_program);
  for (int i = -_render_dist; i < _render_dist + 1; i ++) {
    for (int j = -_render_dist; j < _render_dist + 1; j ++) {
      for (int k = -_render_dist; k < _render_dist + 1; k ++) {
        XYZCoord coord(
            coord_pos.x() + i,
            coord_pos.y() + j,
            coord_pos.z() + k);
        if (!_draw_cache->contains(coord)) continue;
        DrawableSection drawable = _draw_cache->get(coord);

        float x_off = (coord_pos.x() - coord.x()) * 
            World::section_edge;
        float y_off = (coord_pos.y() - coord.y()) *
            World::section_edge;
        float z_off = (coord_pos.z() - coord.z()) *
            World::section_edge;
        glm::vec3 block_offset(x_off, z_off, y_off);
        glm::mat4 model;
        model = glm::translate(model, -block_offset);
        _program->setMatrix("model", model);
  
        drawable.draw();
      }
    }
  }
}

} // namespace cuboc


