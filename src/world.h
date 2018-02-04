#ifndef WORLD_H
#define WORLD_H

#include <memory>

#include <glm/glm.hpp>

#include "loader/programloader.h"
#include "loader/texture_loader.h"
#include "glwrapper/buffer.h"
#include "block.h"

#include "util/hashkey.h"
#include "util/cache.h"
#include "util/section.h"

#define DEFAULT_RENDER_DIST 10
#define BLOCK_SIZE 1.0f
namespace cuboc {

class DrawableSection {
  public:
  DrawableSection(BaseSection<BaseBlock> *section);

  void draw(glm::vec3 offset);
  
  private:
  ArrayObject ao;
};


class World {

  public:
  World(
      graphicsutils::ProgramLoader *program,
      TextureFactory *atlas);

  void draw(XYZCoord coord_pos, glm::vec3 offset);

  private:
  graphicsutils::ProgramLoader *_program;
  TextureFactory *_tex_atlas;

  util::Cache<XYZCoord, BaseSection<Block> > _raw;
  util::Cache<XYZCoord, DrawableSection > *_draw_cache;
};

}


#endif
