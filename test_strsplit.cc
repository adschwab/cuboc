#include <cstdio>

#include "src/util/split.h"

int main() {
  std::string str = "aa__cc";
  
  std::vector<std::string> strs = strutil::split(str, '_');
  for (int i = 0; i < strs.size(); i ++) {
    std::printf("%d. %s\n", i, strs[i].c_str());
  }
  return 0;
}
