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
      std::shared_ptr<std::vector<GLfloat>> heights,
      std::vector<int> tex_ids);

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
  std::shared_ptr<std::vector<GLfloat>> _heights;
  std::vector<int> _tex_ids;
  std::string texture_key;
  std::vector<GLfloat> temp_floats;
};


#endif
