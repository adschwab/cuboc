#ifndef OBJECT_H
#define OBJECT_H

#include <GL/glew.h>
#include <vector>
#include "loader/programloader.h"

struct Attribute {
  int size;
};

class Object {
  public:
  Object(graphicsutils::ProgramLoader *program);

  protected:
  GLuint VAO;
  GLuint VBO;

  GLfloat* _data;
  int _size;
  std::vector<Attribute> _attrs;

  graphicsutils::ProgramLoader *_program;

};

class Box: public Object {

  public:
  Box(graphicsutils::ProgramLoader *program);


};


#endif
