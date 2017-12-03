#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include <string>

namespace Loader {

class Image {

  public:
  Image(std::string file_name, int desired_channels);

  ~Image();
  unsigned char *getBuffer();
  int getWidth();
  int getHeight();
  int getChannels();

  private:

  std::string _file_name;
  unsigned char* data;
  int height;
  int width;
  int original_channels;
  int channels;
};

} // namespace

#endif // inclusion guard
