#include "objects/ground.h"

#include "loader/object.h"

#include <vector>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "loader/programloader.h"
#include "loader/texture_loader.h"

Ground::Ground(
    graphicsutils::ProgramLoader *program,
    TextureFactory *textures,
    int width, int depth,
    std::shared_ptr<const std::vector<GLfloat>> heights,
    int tex_id) :
      Object(program),
      _textures(textures),
      _width(width),
      _depth(depth),
      _heights(heights),
      _tex_id(tex_id) {
  _size = 0;
  
  for (int i = 0; i < _width - 1; i ++) {
    for (int j = 0; j < _depth - 1; j ++) {
      GLfloat x1y1 = (*_heights)[j * _width + i]; 
      GLfloat x2y1 = (*_heights)[(j + 1) * _width + i]; 
      GLfloat x1y2 = (*_heights)[j * _width + i + 1]; 
      GLfloat x2y2 = (*_heights)[(j + 1) * _width + i + 1]; 
      GLfloat cx = (GLfloat)(i + 0.5f);
      GLfloat cy = (GLfloat)(j + 0.5f);
      GLfloat cz = (x1y1 + x2y1 + x1y2 + x2y2)/4.0f;
      //std::printf("%.2f,%.2f,%.2f,%.2f\n", x1y1, x2y1, x1y2, x2y2);
    
      push_vertex((GLfloat)i, x1y1, (GLfloat)j, 0.0f, 0.0f);
      push_vertex((GLfloat)(i + 1), x1y2, (GLfloat)j, 1.0f, 0.0f);
      push_vertex(cx, cz, cy, 0.5f, 0.5f);
      push_vertex((GLfloat)i, x1y1, (GLfloat)j, 0.0f, 0.0f);
      push_vertex((GLfloat)i, x2y1, (GLfloat)(j + 1), 0.0f, 1.0f);
      push_vertex(cx, cz, cy, 0.5f, 0.5f);

      push_vertex((GLfloat)i, x2y1, (GLfloat)(j + 1), 0.0f, 1.0f);

      push_vertex((GLfloat)(i + 1), x2y2, (GLfloat)(j + 1), 1.0f, 1.0f);
      push_vertex(cx, cz, cy, 0.5f, 0.5f);
      push_vertex((GLfloat)(i + 1), x1y2, (GLfloat)j, 1.0f, 0.0f);

      push_vertex((GLfloat)(i + 1), x2y2, (GLfloat)(j + 1), 1.0f, 1.0f);
      push_vertex(cx, cz, cy, 0.5f, 0.5f);
    }

  }
  std::vector<float> data;
  data.assign(temp_floats.data(), temp_floats.data() + temp_floats.size());;

  std::vector<glw::BufferAttr> attr_info;
  glw::BufferAttr vertex;
  vertex.num = 3;
  attr_info.push_back(vertex);
  glw::BufferAttr tex;
  tex.num = 2;
  attr_info.push_back(tex);
  temp_floats.clear();
}

void Ground::set() {
  TextureLoader *texture = _textures->get(_tex_id);
  texture->set(_program);
}

