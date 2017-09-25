#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <string>
#include <unordered_map>
#include <vector>

#include "loader/programloader.h"

class TextureLoader {

  public:
  TextureLoader(std::string filename);
  
  void set(graphicsutils::ProgramLoader *program);

  private:
  GLuint texId;
};


class TextureFactory {

  public:
  TextureFactory(std::string foldername);

  ~TextureFactory() {
    _textures.clear();
  }

  TextureLoader *get(std::string key);

  private:
  std::unordered_map
      <std::string, TextureLoader> _textures;
};

#endif
