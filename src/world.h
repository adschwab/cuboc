#ifndef WORLD_H
#define WORLD_H

#include <memory>

#include <glm/glm.hpp>

#include "loader/programloader.h"
#include "loader/texture_loader.h"
#include "block.h"
#include "drawable_section.h"

#include "util/hashkey.h"
#include "util/cache.h"
#include "util/section.h"

#define DEFAULT_RENDER_DIST 10
#define BLOCK_SIZE 0.5f
namespace cuboc {

class World {

  public:
  World(
      graphicsutils::Program3d *program,
      TextureLoader *atlas);

  void draw();

  static float section_edge;

  private:
  graphicsutils::Program3d *_program;
  TextureLoader *_tex_atlas;

  std::shared_ptr<util::Cache<XYZCoord, std::shared_ptr<BaseSection<Block> > > >
      _raw;
  std::shared_ptr<util::Cache<XYZCoord, DrawableSection > >
      _draw_cache;
};

}


#endif
