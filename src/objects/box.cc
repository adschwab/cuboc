#include "objects/box.h"

#include <memory>

#include "loader/object.h"
#include "objects/data/box_data.h"
#include "loader/programloader.h"
#include "loader/texture_loader.h"

Box::Box(
    graphicsutils::ProgramLoader *program,
    TextureFactory *textures) :
      Object(program),
      _textures(textures),
      texture_key("dirt"){
  std::vector<float> data;
  data.assign(entity::box_data, entity::box_data + 24 * 5);;
  std::vector<unsigned char> indices;
  indices.assign(entity::box_indices, entity::box_indices + 36);;

  std::vector<glw::BufferAttr> attr_info;

  glw::BufferAttr vertex; 
  vertex.num = 3;
  attr_info.push_back(vertex);
  glw::BufferAttr texture;
  texture.num = 2;
  attr_info.push_back(texture);


  glw::VertexBuffer v_buff(data);
  glw::IndexBuffer i_buff(indices);
  array_object = std::unique_ptr<glw::ArrayObject>(
      new glw::ArrayObject(v_buff,i_buff, attr_info));
}

void Box::set() {
  TextureLoader *texture = _textures->get(texture_key);
  texture->set(_program);
}

