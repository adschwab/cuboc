#include "objects/box.h"

#include "loader/object.h"
#include "objects/data/box_data.h"
#include "loader/programloader.h"
#include "loader/texture_loader.h"

Box::Box(
    std::shared_ptr<graphicsutils::ProgramLoader> program,
    TextureFactory *textures) :
      Object(program),
      _textures(textures),
      texture_key("dirt"){
  _data = entity::box_data;
  _size = 36; 
  _stride = 5;

  Attribute vertex;
  vertex.num = 3;
  _attr_info.push_back(vertex);
  Attribute texture;
  texture.num = 2;
  _attr_info.push_back(texture);
  
  init();
}

void Box::set() {
  TextureLoader *texture = _textures->get(texture_key);
  texture->set(_program);
}

