#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "loader/programloader.h"
#include "glwrapper/buffer.h"

class Object {
  public:
  Object(graphicsutils::ProgramLoader *program);
  ~Object();

  virtual void set() = 0;

  virtual void draw(glm::vec3 pos);

  protected:
  std::unique_ptr<glw::ArrayObject> array_object;
  graphicsutils::ProgramLoader *_program;
};


#endif
