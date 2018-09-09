#ifndef PLATFORM_IO_H
#define PLATFORM_IO_H

#include <vector>
#include <string>
#include <memory>

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

namespace io_funcs {

bool is_dir(std::string path) {

  struct stat path_stat;
  stat(path.c_str(), &path_stat);
  return S_ISDIR(path_stat.st_mode);
}

std::shared_ptr<std::vector<std::string>> get_files(
    std::string foldername) {

  std::vector<std::string> list;
  
  DIR* dir = opendir(foldername.c_str());
  struct dirent * entity;
  while ((entity = readdir(dir)) != NULL) {
    std::string subfile = foldername + "/" +
        entity->d_name;
    if (!is_dir(subfile))
      list.push_back(subfile);
  }
  closedir(dir);
  return std::make_shared<std::vector<std::string> >(
      list);
}

inline std::string filename(std::string path) {
  for (int i = path.size() - 1; i >= 0; i --) {
    if (path[i] == '/') {
      return path.substr(i + 1, path.size() - i);
    }
  }
  return path;
}

} // namespace io_funcs

#endif // inclusion guard
