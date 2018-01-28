#include "glwrapper/buffer.h"

glw::VertexBuffer::VertexBuffer(
    std::vector<GLfloat> &data
  ) :
    _size(data.size()) {
  glGenBuffers(1, &this->id);
  this->use();

  glBufferData(GL_ARRAY_BUFFER,
      sizeof(GLfloat) * _size,
      &data.front(), 
      GL_STATIC_DRAW);
}

void glw::VertexBuffer::use() {
  glBindBuffer(GL_ARRAY_BUFFER, this->id);
}

glw::VertexBuffer::~VertexBuffer() {
  glDeleteBuffers(1, &id);
}

int glw::VertexBuffer::size() {
  return _size;
}

void glw::VertexBuffer::draw() {
  glDrawArrays(GL_TRIANGLES, 0, _size);
}



glw::IndexBuffer::IndexBuffer(
    std::vector<GLubyte> &data
  ) :
    _size(data.size()) {
  glGenBuffers(1, &this->id);
  this->use();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
      sizeof(GLubyte) * _size,
      &data.front(),
      GL_STATIC_DRAW);
}

void glw::IndexBuffer::use() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
}

glw::IndexBuffer::~IndexBuffer() {
  glDeleteBuffers(1, &id);
}

int glw::IndexBuffer::size() {
  return _size;
}

void glw::IndexBuffer::draw() {
  glDrawElements(GL_TRIANGLES, _size, GL_UNSIGNED_BYTE, 0);
}



glw::ArrayObject::ArrayObject(
    glw::VertexBuffer vertex_buffer,
    std::vector<BufferAttr> &attrs) :
    _vertex_buffer(vertex_buffer),
    _has_indices(false) {
  glGenVertexArrays(1, &this->id);
  this->use();
  this->_vertex_buffer.use();
  setup(attrs);
  glBindVertexArray(0);
}

glw::ArrayObject::ArrayObject(
    glw::VertexBuffer vertex_buffer,
    glw::IndexBuffer index_buffer,
    std::vector<BufferAttr> &attrs) :
    _vertex_buffer(vertex_buffer),
    _index_buffer(index_buffer),
    _has_indices(true) {
  glGenVertexArrays(1, &this->id);
  this->use();
  this->_vertex_buffer.use();
  this->_index_buffer.use();
  setup(attrs);
  glBindVertexArray(0);
}

glw::ArrayObject::~ArrayObject() {
  glDeleteVertexArrays(1, &this->id);
}

void glw::ArrayObject::use() {
  glBindVertexArray(this->id);
}

void glw::ArrayObject::draw() {
  use();
  if (_has_indices) {
    _index_buffer.draw();
  }
  else {
    _vertex_buffer.draw();
  }
  glBindVertexArray(0);
}


void glw::ArrayObject::setup(std::vector<BufferAttr> &attrs) {
  int stride = 0;
  for (int i = 0; i < attrs.size(); i ++) 
    stride += attrs[i].num;   
 
  int startcnt = 0;
  for (int i = 0; i < attrs.size(); i ++) {
    glw::BufferAttr attr = attrs[i];
    glVertexAttribPointer(
        i,                 //attribute
        attr.num,          //size
        GL_FLOAT, GL_FALSE,
        stride * sizeof(GLfloat),               //stride size
        (GLvoid*)(startcnt * sizeof(GLfloat))); //start loc
    glEnableVertexAttribArray(i);
    startcnt += attr.num;
  }

}
