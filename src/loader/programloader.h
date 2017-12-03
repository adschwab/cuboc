#ifndef PROGRAM_LOADER_H
#define PROGRAM_LOADER_H

#include <string>

#include <glm/glm.hpp>
#include <GL/glew.h>

namespace graphicsutils {

class ProgramLoader {
  public:
  ProgramLoader(std::string v_glsl, std::string f_glsl);

  void use();

  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setMatrix(const std::string &name, glm::mat4 mat) const;
  private:
  GLuint progid;

  std::string load(GLuint& program);

  std::string v_file;
  std::string f_file;

  GLchar error[512];
  
  GLint load_shader(std::string file_name, GLenum shadertype, GLuint& shader);
};

} //namespace graphicsutils

#endif //inclusion guard
