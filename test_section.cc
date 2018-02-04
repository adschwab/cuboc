#include <cstdio>
#include "src/util/section.h"

int main() {
  
  Section<int> section;
  
  std::printf("Edge: %lu\n", section.get_edge());
  std::printf("Size: %lu\n", section.arr.size());
  section.set(4,4,1,1);

  std::printf("%d\n", section.get(4,1,1));

  return 0;
}
