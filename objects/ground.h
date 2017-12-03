#ifndef GROUND_H
#define GROUND_H

#include "loader/object.h"

#include <vector>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "loader/programloader.h"
#include "loader/texture_loader.h"

class Ground: public Object {
  public:
  Ground(graphicsutils::ProgramLoader *program,
      TextureFactory *textures,
      int width, int depth,
      std::shared_ptr<const std::vector<GLfloat> > heights,
      int tex_id);

  virtual void set();

  protected:

  void push_vertex(GLfloat x, GLfloat y, GLfloat z, GLfloat t1, GLfloat t2) {
    temp_floats.push_back(x);
    temp_floats.push_back(y);
    temp_floats.push_back(z);
    temp_floats.push_back(t1);
    temp_floats.push_back(t2);

    _size += 1;
  }
  
  TextureFactory *_textures;
  
  int _width;
  int _depth;
  std::shared_ptr<const std::vector<GLfloat>> _heights;
  int _tex_id;
  std::vector<GLfloat> temp_floats;
};


#endif
