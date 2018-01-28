#ifndef BOX_H
#define BOX_H

#include "loader/object.h"

#include "loader/programloader.h"
#include "loader/texture_loader.h"

class Box: public Object {

  public:
  Box(graphicsutils::ProgramLoader *program,
      TextureFactory *textures);

  virtual void set();

  private:
  TextureFactory *_textures;
  std::string texture_key;
};



#endif
