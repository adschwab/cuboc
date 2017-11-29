#include "loader/object.h"

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "objects/box_data.h"
#include "loader/programloader.h"
#include "loader/texture_loader.h"

Object::Object(graphicsutils::ProgramLoader *program):
    _program(program) {
  
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
}

Object::~Object() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}

void Object::init() {
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  
  int stride = 0;
  for (int i = 0; i < _attr_info.size(); i ++)
    stride += _attr_info[i].num;

  glBufferData(GL_ARRAY_BUFFER,
      sizeof(GLfloat) * _size * stride,
      _data, 
      GL_STATIC_DRAW);

  int startcnt = 0;
  for (int i = 0; i < _attr_info.size(); i ++) {
    Attribute attr = _attr_info[i];
    glVertexAttribPointer(
        i,                 //attribute
        attr.num,          //size
        GL_FLOAT, GL_FALSE,
        stride * sizeof(GLfloat),               //stride size
        (GLvoid*)(startcnt * sizeof(GLfloat))); //start loc
    glEnableVertexAttribArray(i);
    startcnt += attr.num;
  }
  glBindVertexArray(0);
}

void Object::set() {}

void Object::draw(glm::vec3 pos) {
  set();
  glBindVertexArray(VAO);
  glm::mat4 model;
  pos[1] = -pos[1];
  model = glm::translate(model, -pos);
  _program->setMatrix("model", model);
  glDrawArrays(GL_TRIANGLES, 0, _size);
  glBindVertexArray(0);
}


Box::Box(
    graphicsutils::ProgramLoader *program,
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

Ground::Ground(graphicsutils::ProgramLoader *program,
    TextureFactory *textures,
    int width, int depth,
    std::vector<GLfloat> heights,
    std::vector<int> tex_ids) :
      Object(program),
      _textures(textures),
      _width(width),
      _depth(depth),
      _heights(heights),
      _tex_ids(tex_ids),
      texture_key("dirt") {
  _size = 0;
  _stride = 3;
  
  for (int i = 0; i < _width - 1; i ++) {
    for (int j = 0; j < _depth - 1; j ++) {
      GLfloat x1y1 = _heights[j * _width + i];
      GLfloat x2y1 = _heights[(j + 1) * _width + i];
      GLfloat x1y2 = _heights[j * _width + i + 1];
      GLfloat x2y2 = _heights[(j + 1) * _width + i + 1];
      GLfloat cx = (GLfloat)(i + 0.5f);
      GLfloat cy = (GLfloat)(j + 0.5f);
      GLfloat cz = (x1y1 + x2y1 + x1y2 + x2y2)/4.0f;
      //std::printf("%.2f,%.2f,%.2f\n", x1y1, x2y1, x1y2);
      //std::printf("%.2f,%.2f,%.2f\n", x2y1, x1y2, x2y2);
      
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
  _data = temp_floats.data();
 
  Attribute vertex;
  vertex.num = 3;
  _attr_info.push_back(vertex);
  Attribute tex;
  tex.num = 2;
  _attr_info.push_back(tex);
  init();
  temp_floats.clear();
}

void Ground::set() {
  TextureLoader *texture = _textures->get(texture_key);
  texture->set(_program);
}

