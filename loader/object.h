#ifndef OBJECT_H
#define OBJECT_H

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "loader/programloader.h"
#include "loader/texture_loader.h"

struct Attribute {
  int num;
};

class Object {
  public:
  Object(graphicsutils::ProgramLoader *program);
  ~Object();

  void init();

  virtual void set();
  virtual void draw(glm::vec3 pos);

  protected:
  GLuint VAO;
  GLuint VBO;

  GLfloat* _data;
  int _size;
  int _stride;
  graphicsutils::ProgramLoader *_program;
  std::vector<Attribute> _attr_info;
};


#endif
