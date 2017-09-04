#ifndef BUFFER_H
#define BUFFER_H

#include <vector>

#include <GL/glew.h>

class BufferStore {
  public:
  BufferStore() {
  }

  inline unsigned long size() { return blists.size(); }

  struct BufferList {
    GLuint *vao;
    GLuint *vbo;
    GLuint *ebo;
  };
  
  unsigned long addBuffer();

  void bindBuffer(unsigned long index, GLfloat *vertices,
      int num_vertices, GLuint *indices, int num_indices);
  
  void draw(unsigned long index, int num);
  private:
  std::vector<BufferList> blists;
};




#endif // Inclusion guard
