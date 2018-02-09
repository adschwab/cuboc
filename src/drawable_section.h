#ifndef DRAWABLE_SECTION_H
#define DRAWABLE_SECTION_H

#include <glm/glm.hpp>

#include "glwrapper/buffer.h"
#include "block.h"

#include "util/section.h"

namespace cuboc {

class DrawableSection {
  public:
  DrawableSection() {}
  DrawableSection(
      BaseSection<Block> *section, float size);

  void draw(glm::vec3 offset);
  
  private:
  std::shared_ptr<glw::ArrayObject> ao_;


  // Statics
  static float tex_arr_x[TEX_WIDTH][4];
  static float tex_arr_y[TEX_WIDTH][4];  
  static bool static_init;

  static void add_bottom_top(
      std::vector<float> &v,
      std::vector<unsigned char> &ind,
      float x, float y, float z,
      float sz,
      char tex_type, short tex_row,
      char rotation);
  
  static void add_left_right(
      std::vector<float> &v,
      std::vector<unsigned char> &ind,
      float x, float y, float z,
      float sz,
      char tex_type, short tex_row,
      char rotation);
  
  static void add_front_back(
      std::vector<float> &v,
      std::vector<unsigned char> &ind,
      float x, float y, float z,
      float sz,
      char tex_type, short tex_row,
      char rotation);
  

  static void add_vertex(
      std::vector<float> &vertices,
      float px,
      float py,
      float pz,
      float xtex,
      float ytex);

  static void add_triangle(
      std::vector<unsigned char> &inds,
      unsigned char ind1,
      unsigned char ind2,
      unsigned char ind3);
};

}


#endif
