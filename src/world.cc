#include "world.h"

#include <vector>
#include <unordered_map>

#include "gen/ground_gen.h"
#include "objects/ground.h"

#include "loader/programloader.h"
#include "loader/texture_loader.h"

World::World(graphicsutils::ProgramLoader *program,
    TextureFactory *textures) :
      _program(program), _textures(textures) {
  init();
}

void World::init() {
  for (int i = 0; i < 2; i ++) {
    for (int j = 0; j < 2; j ++) {
      XYCoord coord((float)BLOCK_SIZE * i,
          (float)BLOCK_SIZE * j);
      std::shared_ptr<std::vector<float> > heights = 
          gen_ground_data(BLOCK_SIZE, BLOCK_SIZE);
      Block block(heights);
      _blocks.insert(std::make_pair(coord, block));
    }
  }
}

void World::draw() {
  for (auto it = _blocks.begin(); it != _blocks.end(); ++it) {
    Block block = it->second;
    Ground ground(_program, _textures, BLOCK_SIZE, BLOCK_SIZE,
        block.get(),
        1);
    ground.draw(it->first.vec());
    
    XYCoord x(it->first.x + (float)BLOCK_SIZE, it->first.y);
        
  }
}
