#include <cstdio>
#include "src/util/section.h"

int main() {
  
  Section<int> section;
  section.set(4,1,1,1);
  std::printf("%d\n", section.get(1,1,1));
  return 0;
}
