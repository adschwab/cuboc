

#include "loader/image_loader.h"
#define STB_IMAGE_IMPLEMENTATION

int main() {
  Loader::Image image("textures/brick_wall.jpg", 0);
  return 0;
}
