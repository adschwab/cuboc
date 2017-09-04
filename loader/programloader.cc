#include "programloader.h"

#include <string>
#include <cstdlib>

#include <GL/glew.h>

#include "util/loadfile.h"

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

void ProgramLoader::setInt(const std::string &name, int value) const {
  GLint loc = glGetUniformLocation(progid, name.c_str());
  glUniform1i(loc, value);
} 

void ProgramLoader::setFloat(const std::string &name, float value) const {
  GLint loc = glGetUniformLocation(progid, name.c_str());
  glUniform1f(loc, value);
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

  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
    return std::string("Error linking shaders: ") + std::string(error);

  glDeleteShader(vertex);
  glDeleteShader(fragment);

  return "OK";
}

GLint ProgramLoader::load_shader(std::string file_name, GLenum shadertype, GLuint& shader) {
  
  const std::string fstr = utils::loadFromFile(file_name); 
  const char *fcstr = fstr.c_str();

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

} //namespace graphicsutils
