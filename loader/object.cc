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
      texture_key("container"){
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
