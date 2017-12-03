#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include <vector>
#include <memory>

#include "loader/programloader.h"
#include "loader/texture_loader.h"

#include "objects/ground.h"
#include "util/hashkey.h"

#define BLOCK_SIZE 8

class Block {
  public:
  Block(std::shared_ptr<std::vector<float> > heights):
    _heights(heights) {}

  ~Block() {}

  std::shared_ptr<const std::vector<float> > get() {
    return _heights;
  }

  private:
  std::shared_ptr<std::vector<float> > _heights;

};

class World {

  public:
  World(graphicsutils::ProgramLoader *program,
      TextureFactory *textures);

  void init();

  void draw();

  private:
  graphicsutils::ProgramLoader *_program;
  TextureFactory *_textures;

  std::unordered_map<XYCoord, Block> _blocks;

};




#endif
