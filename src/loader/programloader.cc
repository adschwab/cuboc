#include "programloader.h"

#include <string>
#include <cstdlib>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

#include "util/loadfile.h"
#include "util/log.h"

namespace graphicsutils {
  
ProgramLoader::ProgramLoader(std::string v_glsl, std::string f_glsl) : v_file(v_glsl), f_file(f_glsl) {
  std::string result = load(progid);
  if (result != "OK") {
    std::fprintf(stderr, "Fatal %s\n", result.c_str());
    std::exit(1);
  }
}

void ProgramLoader::use() {
  glUseProgram(progid);
}

void ProgramLoader::done() {
  glUseProgram(0);
}

void ProgramLoader::setInt(const std::string &name, int value) const {
  GLint loc = glGetUniformLocation(progid, name.c_str());
  glUniform1i(loc, value);
} 

void ProgramLoader::setFloat(const std::string &name, float value) const {
  GLint loc = glGetUniformLocation(progid, name.c_str());
  glUniform1f(loc, value);
}

void ProgramLoader::setMatrix(
    const std::string &name,
    glm::mat4 mat) const {
  GLint loc = glGetUniformLocation(progid, name.c_str());
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void ProgramLoader::setVec(
    const std::string &name,
    glm::vec3 vec) const {
  GLint loc = glGetUniformLocation(progid, name.c_str());
  glUniform3f(loc, vec[0], vec[1], vec[2]);
}

std::string ProgramLoader::load(GLuint& program) {
  GLuint vertex;
  GLuint fragment;
  
  GLint success = load_shader(v_file, GL_VERTEX_SHADER, vertex);
  if (!success)
    return std::string("Error loading vertex shader: ") + std::string(error);

  success = load_shader(f_file, GL_FRAGMENT_SHADER, fragment);
  if (!success)
    return std::string("Error loading fragment shader: ") + std::string(error);

  program = glCreateProgram();
  
  glAttachShader(program, vertex);

  glAttachShader(program, fragment);
  glLinkProgram(program);


  glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
  if (!success)
    glGetProgramInfoLog(program, 512, NULL, error);
    return std::string("Error linking shaders: ") + std::string(error);
  
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
    glGetProgramInfoLog(program, 512, NULL, error);
    return std::string("Error linking shaders: ") + std::string(error);


  glDeleteShader(vertex);
  glDeleteShader(fragment);

  return "OK";
}

GLint ProgramLoader::load_shader(std::string file_name, GLenum shadertype, GLuint& shader) {
  
  const std::string fstr = utils::loadFromFile(file_name); 

  const char *fcstr = fstr.c_str();
  LOGF("%s", fcstr);

  shader = glCreateShader(shadertype);

  glShaderSource(shader, 1, &fcstr, NULL);
  glCompileShader(shader);

  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, error);
  }
  
  return success;
}

Program3d::Program3d(
    std::string v_glsl,
    std::string f_glsl,
    cuboc::Camera *camera) :
      ProgramLoader(v_glsl, f_glsl),
      _camera(camera) {
  this->use();
  this->setMatrix("projection", _camera->getProj());
  glUseProgram(0);
}

void Program3d::use() {
  ProgramLoader::use();
  this->setMatrix("view", _camera->getView());
}

} //namespace graphicsutils
