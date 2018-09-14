#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <string>
#include <unordered_map>
#include <vector>

#include "loader/programloader.h"

class TextureLoader {

  public:
  TextureLoader(std::string filename, bool is_tex);
  
  void set(graphicsutils::ProgramLoader *program);

  protected:
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
