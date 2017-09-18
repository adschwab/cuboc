#include "loader/texture_loader.h"

#include <string>

#include <GL/glew.h>

#include "loader/image_loader.h"
#include "loader/programloader.h"

TextureLoader::TextureLoader(std::string filename) {

  glGenTextures(1, &texId);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texId);

  glTexParameteri(GL_TEXTURE_2D,
      GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,
      GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,
      GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,
      GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  Loader::Image texImg = 
      Loader::Image(filename, 3);
  void *data = (void *)texImg.getBuffer();
  glTexImage2D(GL_TEXTURE_2D, 0,
      GL_RGB,
      texImg.getWidth(),
      texImg.getHeight(),
      0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureLoader::init(
    graphicsutils::ProgramLoader *program) {
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texId);
  program->use();
  program->setInt("texture1", 1);
}

