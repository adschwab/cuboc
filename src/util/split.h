#include <vector>
#include <string>

namespace strutil {

std::vector<std::string> split(std::string str, char chr) {

  std::vector<std::string> array;
  int lap = 0;
  for (int i = 0; i < str.size(); i ++) {
    if (str[i] == chr) {
      array.push_back(str.substr(lap, i - lap));
      lap = i + 1;
    }
  }
  array.push_back(str.substr(lap, str.size() - lap));
  return array;
}

}
