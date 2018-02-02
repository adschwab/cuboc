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
      texture_key("grounds_atlas"){
  int tex_x_sz = 2;
  int tex_y_sz = 3;
  std::vector<float> data;
  data.assign(entity::box_data, entity::box_data + 24 * 5);;
  for (int i = 0; i < data.size()/5; i ++) {
    data[i * 5 + 3] /= tex_x_sz;
    data[i * 5 + 4] /= tex_y_sz;

    data[i * 5 + 3] += entity::texture_map[i * 2] / (float)tex_x_sz;
    data[i * 5 + 4] += (1.0f - (tex_y_sz - entity::texture_map[i * 2 + 1]) / (float)tex_y_sz);
  }
  
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

