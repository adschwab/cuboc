
#include <cstdio>
#include <vector>

#include "util/circle.h"

int main() {
  
  XYCoord origin = XYCoord(0.5f, 0.0f);
  float radius = 300.0f;
  int block_size = 2;
  std::vector<XYCoord> circle;
  
  genCircle(
      origin,
      radius,
      block_size,
      circle);
  std::printf("%lu\n", circle.size());

  //for (int i = 0; i < circle.size(); i ++) {
  //  XYCoord coord = circle[i];
  //  std::printf("(%f,%f)\n", coord.x(), coord.y());
  //}
  return 0;
}
