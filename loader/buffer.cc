#include "buffer.h"

#include <cstdlib>
#include <vector>

#include <GL/glew.h>


  
unsigned long BufferStore::addBuffer() {
 
  GLuint *vao = (GLuint *) std::malloc(sizeof(GLuint));
  GLuint *vbo = (GLuint *) std::malloc(sizeof(GLuint));
  GLuint *ebo = (GLuint *) std::malloc(sizeof(GLuint));

  glGenVertexArrays(1, vao);
  glGenBuffers(1, vbo);
  glGenBuffers(1, ebo);

    
  BufferList buffer = {vao, vbo, ebo};

  blists.push_back(buffer);

  return size() - 1;
}

void BufferStore::bindBuffer(unsigned long index, GLfloat *vertices, 
    int num_vertices, GLuint *indices, int num_indices) {
  
  BufferList buffer = blists[index];

  glBindVertexArray(*buffer.vao);
  glBindBuffer(GL_ARRAY_BUFFER, *buffer.vbo);
  glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

  if (indices != NULL) {

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *buffer.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(GLuint), indices, GL_STATIC_DRAW);
  }
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
  
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

}

void BufferStore::draw(unsigned long index, int num) {
  BufferList buffer = blists[index];

  glBindVertexArray(*buffer.vao);
  glDrawArrays(GL_TRIANGLES, 0, num);
  glBindVertexArray(0);
}

