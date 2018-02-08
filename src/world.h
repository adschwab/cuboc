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
  DrawableSection(
      std::shared_ptr<BaseSection<BaseBlock> > section);

  void draw(glm::vec3 offset);
  
  private:
  static float tex_arr_x[TEX_WIDTH][4];
  static float tex_arr_y[TEX_WIDTH][4];  
  static bool static_init;

  void add_bottom_top(
      std::vector<float> &v,
      std::vector<unsigned char> &ind,
      float x, float y, float z,
      float sz,
      char tex_type, short tex_row,
      char rotation);
  
  void add_left_right(
      std::vector<float> &v,
      std::vector<unsigned char> &ind,
      float x, float y, float z,
      float sz,
      char tex_type, short tex_row,
      char rotation);
  
  void add_front_back(
      std::vector<float> &v,
      std::vector<unsigned char> &ind,
      float x, float y, float z,
      float sz,
      char tex_type, short tex_row,
      char rotation);
  

  void add_vertex(
      std::vector<float> &vertices,
      float px,
      float py,
      float pz,
      float xtex,
      float ytex);

  void add_triangle(
      std::vector<unsigned char> &inds,
      unsigned char ind1,
      unsigned char ind2,
      unsigned char ind3);

  glw::ArrayObject ao;
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

  util::Cache<XYZCoord, BaseSection<BaseBlock> > _raw;
  util::Cache<XYZCoord, DrawableSection > *_draw_cache;
};

}


#endif
