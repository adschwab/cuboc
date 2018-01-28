#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include <memory>

#include <GL/glew.h>

namespace glw {

struct BufferAttr {
  int num;
};

class VertexBuffer {
  public:
  VertexBuffer(std::vector<GLfloat> &data);
  ~VertexBuffer();

  int size();

  void use();

  void draw();
  
  private:
  GLuint id;

  int _size;
};

class IndexBuffer {
  public:
  IndexBuffer() {}
    
  IndexBuffer(
      std::vector<GLubyte> &data);
  ~IndexBuffer();

  int size();

  void use();
  
  void draw();

  private:
  GLuint id;

  int _size;
};

class ArrayObject {
  public:
  ArrayObject(
      VertexBuffer vertex_buffer,
      std::vector<BufferAttr> &attrs);

  ArrayObject(
      VertexBuffer vertex_buffer,
      IndexBuffer index_buffer,
      std::vector<BufferAttr> &attrs);
  ~ArrayObject();

  void use();
  void draw();

  private:
  bool _has_indices;
  GLuint id;

  void setup(std::vector<BufferAttr> &attrs);

  VertexBuffer _vertex_buffer;
  IndexBuffer _index_buffer;
};

}

#endif
