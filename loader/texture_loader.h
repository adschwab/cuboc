#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <string>

#include "loader/programloader.h"

class TextureLoader {

  public:
  TextureLoader(std::string filename);

  void use(graphicsutils::ProgramLoader *program);

  private:
  GLuint texId;
};

#endif
