#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include <memory>


namespace glw {

struct BufferAttr {
  int num;
};

class VertexBuffer {
  public:
  VertexBuffer(std::vector<float> &data);
  ~VertexBuffer();

  int size();

  void use();

  void draw();
  
  private:
  unsigned int id;

  int _size;
};

class IndexBuffer {
  public:
  IndexBuffer() {}
    
  IndexBuffer(
      std::vector<unsigned char> &data);
  ~IndexBuffer();

  int size();

  void use();
  
  void draw();

  private:
  unsigned int id;

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
  unsigned int id;

  void setup(std::vector<BufferAttr> &attrs);

  VertexBuffer _vertex_buffer;
  IndexBuffer _index_buffer;
};

}

#endif
