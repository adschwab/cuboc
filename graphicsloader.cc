#include "graphicsloader.h"

#include <GL/glew.h>

namespace glib {

int Triangles::add_triangle(float p1, float p2, float p3) {
  triangle new_tri = {p1, p2, p3};

  int last = triangles.size() * 3;
  if (last + 2 > sizeof(vertices)/sizeof(GLfloat)) {
    doubleArray();
  }
  vertices[last] = p1;
  vertices[last + 1] = p2;
  vertices[last + 2] = p3;
  
  triangles.push_back(new_tri);
  return triangles.size() - 1;
}

std::vector<float> Triangles::get(int i) {
  triangle t = triangles[i];
  std::vector<float> pts;
  pts.push_back(t.p1);
  pts.push_back(t.p2);
  pts.push_back(t.p3);
  return pts;
}

void Triangles::doubleArray() {
  GLfloat nvertices[arr_size * 2];
  for (int i = 0; i < arr_size; i ++) {
    nvertices[i] = vertices[i];
  }
  vertices = &nvertices[0];
}


} // namespace glib
