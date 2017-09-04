#ifndef GRAPHICS_LOADER_H
#define GRAPHICS_LOADER_H

#include <vector>
#include <GL/glew.h>

namespace glib {

class Triangles {

  public:
    Triangles() {
      arr_size = 10;
      GLfloat arr[10];
      vertices = &arr[0];
    }
    
    int add_triangle(float p1, float p2, float p3);
    
    std::vector<float> get(int i);

  private:

    struct triangle {
      float p1;
      float p2;
      float p3;
    };

    std::vector<triangle> triangles;
    
    void doubleArray();
    
    GLfloat *vertices;
    int arr_size;
};

} // namespace glib
#endif // inclusion guard
