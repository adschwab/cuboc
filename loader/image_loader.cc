#include "image_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

namespace Loader {

Image::Image(std::string file_name, int desired_channels):
    _file_name(file_name), channels(desired_channels) {
  data = stbi_load(
      _file_name.c_str(),
      &width, 
      &height, 
      &original_channels,
      desired_channels);
  if (desired_channels == 0) {
    channels = original_channels;
  }
  std::printf("%dX%dX%d\n", width, height, channels);
}

Image::~Image() {
  stbi_image_free(data);
}

unsigned char *Image::getBuffer() {
  return data;
}

int Image::getWidth() {
  return width;
}

int Image::getHeight() {
  return height;
}

int Image::getChannels() {
  return channels;
}

} // namespace
