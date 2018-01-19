#include "loader/object.h"

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "loader/programloader.h"

Object::Object(
    std::shared_ptr<graphicsutils::ProgramLoader> program):
      _program(program) {
  
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glGenVertexArrays(1, &VAO);

  _isize = 0;
}

Object::~Object() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
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

  if (_isize) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLubyte) * _isize,
        _indices,
        GL_STATIC_DRAW);
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
  if (_isize)
    glDrawElements(GL_TRIANGLES, _isize, GL_UNSIGNED_BYTE, 0);
  else
    glDrawArrays(GL_TRIANGLES, 0, _size);

  glBindVertexArray(0);
}


