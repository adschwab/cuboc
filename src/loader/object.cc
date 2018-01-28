#include "loader/object.h"

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "loader/programloader.h"

Object::Object(
    graphicsutils::ProgramLoader *program):
      _program(program) {
}

Object::~Object() {}

void Object::set() {}

void Object::draw(glm::vec3 pos) {
  set();
  glm::mat4 model;
  pos[1] = -pos[1];
  model = glm::translate(model, -pos);
  _program->setMatrix("model", model);
  array_object->draw();
}


