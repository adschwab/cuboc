#ifndef OBJECT_H
#define OBJECT_H

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "loader/programloader.h"
#include "loader/texture_loader.h"

struct Attribute {
  int num;
};

class Object {
  public:
  Object(graphicsutils::ProgramLoader *program);
  ~Object();

  void init();

  virtual void set();
  virtual void draw(glm::vec3 pos);

  protected:
  GLuint VAO;
  GLuint VBO;

  GLfloat* _data;
  int _size;
  int _stride;
  graphicsutils::ProgramLoader *_program;
  std::vector<Attribute> _attr_info;
};

class Box: public Object {

  public:
  Box(graphicsutils::ProgramLoader *program,
      TextureFactory *textures);

  virtual void set();

  private:
  TextureFactory *_textures;
  std::string texture_key;
};


class Ground: public Object {
  public:
  Ground(graphicsutils::ProgramLoader *program,
      TextureFactory *textures,
      int width, int depth,
      std::vector<GLfloat> heights,
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
  std::vector<GLfloat> _heights;
  std::vector<int> _tex_ids;
  std::string texture_key;
  std::vector<GLfloat> temp_floats;
};

#endif
