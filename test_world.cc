#include <cstdio>


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "window.h"
#include "loader/programloader.h"
#include "loader/object.h"
#include "world.h"

int main() {
  base::Window window(800, 600, "Hello");
  glewInit();
  graphicsutils::ProgramLoader program("shaders/vertex.glsl", "shaders/fragment.glsl");
  Object obj(&program);
  World world;
  world.set(1, 2,
            1, 2,
            1, 2,
            obj);
  return 0;
}
