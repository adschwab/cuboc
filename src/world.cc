#include "world.h"

#include <vector>

#include <glm/glm.hpp>

#include "gen/ground_gen.h"
#include "objects/ground.h"

#include "loader/programloader.h"
#include "loader/texture_loader.h"

void GroundObj::draw() {

  main->draw(main_pos.vec());
  if (x)
    x->draw(xpos.vec());
  if (y)
    y->draw(ypos.vec());
  if (xy)
    xy->draw(xypos.vec());
}

World::World(
    graphicsutils::ProgramLoader *program,
    TextureFactory *textures,
    float render_dist) :
      _program(program),
      _textures(textures),
      _render_dist(render_dist) {
  init();
}

void World::init() {
  for (int i = 0; i < 2; i ++) {
    for (int j = 0; j < 2; j ++) {
      XYCoord coord((float)BLOCK_SIZE * i,
          (float)BLOCK_SIZE * j);
      std::shared_ptr<std::vector<float> > heights =
          std::make_shared<std::vector<float> >();
      std::shared_ptr<std::vector<int> > tex_ids =
          std::make_shared<std::vector<int> >();
      gen_ground_data(_blocks, heights, tex_ids);
      Block block(heights, tex_ids);
      _blocks.insert(std::make_pair(coord, block));
    }
  }
}

void World::draw(glm::vec3 vec) {
  XYCoord cam(vec);
  for (auto it = _blocks.begin(); it != _blocks.end(); ++it) {

    XYCoord key = it->first;

    XYCoord xneighbor(key.x() + (float)BLOCK_SIZE, key.y());
    XYCoord yneighbor(key.x(), key.y() + (float)BLOCK_SIZE);
    XYCoord xyneighbor(
        key.x() + (float)BLOCK_SIZE,
        key.y() + (float)BLOCK_SIZE);
 
    std::shared_ptr<const std::vector<float> > xheights;
    std::shared_ptr<const std::vector<float> > yheights; 


    Block block = it->second;
    auto heights = block.get();

    std::shared_ptr<GroundObj> ground_obj;
    ground_obj = _cache.get(key);

    if (!_cache.contains(key) || !ground_obj) {
      std::shared_ptr<Ground> ground = 
          std::make_shared<Ground>(
              _program,
              _textures,
              BLOCK_SIZE,
              BLOCK_SIZE,
              heights,
              block.getMainTex());
      ground_obj = std::make_shared<GroundObj>();
      ground_obj->main = ground;
      ground_obj->main_pos = key;
    }
    
    auto xiter = _blocks.find(xneighbor);
    if (!ground_obj->x && xiter != _blocks.end()) {
      xheights = xiter->second.get();
      auto newheights = std::make_shared<std::vector<float> >();
      XYCoord newcoord(key.x() + 1, key.y());
      for (int i = 0; i < BLOCK_SIZE; i ++) {
        newheights->push_back(
            (*xheights)[i * BLOCK_SIZE + BLOCK_SIZE - 1]);
        newheights->push_back((*heights)[i * BLOCK_SIZE]);
      }
      std::shared_ptr<Ground> xground = 
          std::make_shared<Ground>(
              _program,
              _textures,
              2,
              BLOCK_SIZE,
              newheights,
              block.getXTex());
      ground_obj->x = xground;
      ground_obj->xpos = newcoord;
    }


    auto yiter = _blocks.find(yneighbor);
    if (!ground_obj->y && yiter != _blocks.end()) {
      yheights = yiter->second.get();
      auto newheights = std::make_shared<std::vector<float> >();
      XYCoord newcoord(key.x(), key.y() + 1);
      for (int i = 0; i < BLOCK_SIZE; i ++) {
        newheights->push_back(
            (*yheights)[BLOCK_SIZE * (BLOCK_SIZE - 1) + i]);
      }
      for (int i = 0; i < BLOCK_SIZE; i ++) {
        newheights->push_back((*heights)[i]);
      }
      std::shared_ptr<Ground> yground =
          std::make_shared<Ground>(
              _program,
              _textures,
              BLOCK_SIZE,
              2,
              newheights,
              block.getYTex());
      ground_obj->y = yground;
      ground_obj->ypos = newcoord;
    }

    auto xyiter = _blocks.find(xyneighbor);
    if (!ground_obj->xy
        && xheights
        && yheights
        && xyiter != _blocks.end()) {
      auto xyheights = xyiter->second.get();
      auto newheights = std::make_shared<std::vector<float> >();
      XYCoord newcoord(key.x() + 1, key.y() + 1);
      newheights->push_back(
          (*xyheights)[BLOCK_SIZE * BLOCK_SIZE - 1]);
      newheights->push_back(
          (*yheights)[BLOCK_SIZE * (BLOCK_SIZE - 1)]);
      newheights->push_back(
          (*xheights)[BLOCK_SIZE - 1]);
      newheights->push_back(
          (*heights)[0]);
      std::shared_ptr<Ground> xyground =
          std::make_shared<Ground>(
              _program,
              _textures,
              2,
              2,
              newheights,
              block.getXYTex());
      ground_obj->xy = xyground;
      ground_obj->xypos = newcoord;
    }

    ground_obj->draw();
    _cache.add(key, ground_obj);
  }
}
