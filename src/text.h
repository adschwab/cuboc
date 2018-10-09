#ifndef TEXT_H
#define TEXT_H

#include <string>

#include "glwrapper/buffer.h"
#include "loader/programloader.h"
#include "loader/texture_loader.h"

class DrawText {
 public:
  DrawText(graphicsutils::ProgramLoader *img_program);
  
  void draw(std::string str, float left, float top, float pt, float aspectRatio);
 
 private:
  graphicsutils::ProgramLoader *img_program_;

  TextureLoader text_img_;
};

#endif
