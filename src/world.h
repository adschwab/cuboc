#ifndef WORLD_H
#define WORLD_H

#include <memory>

#include <glm/glm.hpp>

#include "loader/programloader.h"
#include "loader/texture_loader.h"
#include "block.h"
#include "drawable_section.h"
#include "camera.h"

#include "util/hashkey.h"
#include "util/cache.h"
#include "util/section.h"

#define DEFAULT_RENDER_DIST 5
#define BLOCK_SIZE 0.5f
namespace cuboc {

class World {

  public:
  World(
      graphicsutils::Program3d *program,
      TextureLoader *atlas,
      Camera *camera,
      unsigned char render_dist=DEFAULT_RENDER_DIST);

  void draw();

  static float block_size;
  static float section_edge;
  static int section_count;
  static int getSubIndex(float offset);
  static float getSubOffset(int index);

  util::Store<XYZCoord, std::shared_ptr<BaseSection<Block> > > *get_raw();

  private:
  graphicsutils::Program3d *_program;
  TextureLoader *_tex_atlas;
  unsigned char _render_dist;
  Camera *_camera;

  util::Cache<XYZCoord, std::shared_ptr<BaseSection<Block> > > _raw;
  util::Cache<XYZCoord, DrawableSection > _draw_cache;
};

}


#endif
