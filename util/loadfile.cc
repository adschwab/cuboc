#include "loadfile.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

namespace utils {
const std::string loadFromFile(std::string filename) {
  std::stringstream stream;
  std::ifstream file(filename);
  stream << file.rdbuf();
  file.close();
  return stream.str();
}
} // namespace utils

