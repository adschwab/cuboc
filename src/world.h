#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "loader/programloader.h"
#include "loader/texture_loader.h"

#include "objects/ground.h"
#include "util/hashkey.h"
#include "util/cache.h"

#define BLOCK_SIZE 2
#define DEFAULT_RENDER_DIST 10

struct GroundObj {

  std::shared_ptr<Ground> main;
  XYCoord main_pos;
  std::shared_ptr<Ground> x;
  XYCoord xpos;
  std::shared_ptr<Ground> y;
  XYCoord ypos;
  std::shared_ptr<Ground> xy;
  XYCoord xypos;

  void draw();
};

class Block {
  public:
  Block(
      std::shared_ptr<std::vector<float> > heights,
      std::shared_ptr<std::vector<int> > tex_ids):
    _heights(heights), _tex_ids(tex_ids) {}

  ~Block() {}

  std::shared_ptr<const std::vector<float> > get() {
    return _heights;
  }
  
  int getMainTex() {
    return (*_tex_ids)[0];
  }

  int getXTex() {
    return (*_tex_ids)[1];
  }

  int getYTex() {
    return (*_tex_ids)[2];
  }

  int getXYTex() {
    return (*_tex_ids)[3];
  }
  
  private:
  std::shared_ptr<std::vector<float> > _heights;
  std::shared_ptr<std::vector<int> > _tex_ids;

};

class World {

  public:
  World(
      graphicsutils::ProgramLoader *program,
      TextureFactory *textures,
      float render_dist=DEFAULT_RENDER_DIST);

  void init();

  void draw(glm::vec3 vec);

  private:
  graphicsutils::ProgramLoader *_program;
  TextureFactory *_textures;

  std::unordered_map<XYCoord, Block> _blocks;
  util::Cache<XYCoord, GroundObj > _cache;

  float _render_dist;
};




#endif
