#include "loader/object.h"

#include <vector>

#include <GL/glew.h>

#include "objects/box_data.h"
#include "loader/programloader.h"

Object::Object(graphicsutils::ProgramLoader *program):
    _program(program) {
  
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
}


Box::Box(graphicsutils::ProgramLoader *program) : 
    Object(program) {
  _data = entity::box_data;
  Attribute vertices;
  vertices.size = 3;
  Attribute tex;
  tex.size = 2;
  _attrs.push_back(vertices);
  _attrs.push_back(tex);

  _size = 0;
  for (int i = 0; i < _attrs.size(); i++)
    _size += _attrs[i].size;
}

