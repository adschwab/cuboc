#include "src/util/hashalg.h"
#include <cstdio>

int main() {

  unsigned int a[3] = {1, 2, 0};

  ub8 h = hash((void *)a, 12);
  std::printf("%llu\n", h);
  return 0;
}
