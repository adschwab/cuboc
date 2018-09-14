#include "loader/texture_loader.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>

#include <GL/glew.h>

#include "platform/io_funcs.h"
#include "loader/image_loader.h"
#include "loader/programloader.h"
#include "util/split.h"


TextureLoader::TextureLoader(std::string filename, bool is_tex) {

  if (glewInit() != GLEW_OK) {
    std::fprintf(stderr,
        "Fatal error initializing GLEW. Aborting.\n");
    std::exit(EXIT_FAILURE);
  }
  glGenTextures(1, &texId);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texId);


  glTexParameteri(GL_TEXTURE_2D,
      GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,
      GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,
      GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,
      GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  Loader::Image texImg = 
      Loader::Image(filename, 3);
  void *data = (void *)texImg.getBuffer();
  glTexImage2D(GL_TEXTURE_2D, 0,
      GL_RGB,
      texImg.getWidth(),
      texImg.getHeight(),
      0, GL_RGB, GL_UNSIGNED_BYTE, data);
  if (is_tex) {
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureLoader::set(
    graphicsutils::ProgramLoader *program) {
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texId);
  program->use();
  program->setInt("texture1", 1);
}


//================== FACTORY =================

TextureFactory::TextureFactory(std::string foldername) {
  std::shared_ptr<std::vector<std::string> > filelist = 
    io_funcs::get_files(foldername);

  for (int i = 0; i < filelist->size(); i ++) {
    std::string name = (*filelist)[i];
    std::string filename = io_funcs::filename(name);
    int ext_index = -1;
    for (int i = filename.size() - 1; i >= 0; i--) {
      if (filename[i] == '.') {
        ext_index = i;
      }
    }
    if (ext_index == -1)
      continue;
    std::string key = filename.substr(0, ext_index);
    std::string ext = filename.substr(ext_index + 1,
        filename.size() - ext_index);
    std::printf("LOADING %s\n", key.c_str());
      
    TextureLoader texture(name, true);
    _textures.insert(std::make_pair(key, texture));
  }
}

TextureLoader *TextureFactory::get(std::string key) {
  auto value = _textures.find(key);
  if (value == _textures.end()) {
    std::printf(
        "Error in texture configuration - '%s'\n",
        key.c_str());
    std::exit(1);
  }
  return &value->second;
}

