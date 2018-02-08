#include "world.h"

#include <vector>

#include <glm/glm.hpp>

#include "loader/programloader.h"
#include "loader/texture_loader.h"
#include "block.h"

namespace cuboc {

DrawableSection::static_init = false;
  
void DrawableSection::add_bottom_top(
    std::vector<float> &v, 
    std::vector<unsigned char> &ind,
    float x, float y, float z,
    float sz, 
    char tex_type, short tex_row,
    char rotation) {
  auto curr_ind = v.size();
  short tex_offset = (TEX_HEIGHT - tex_row - 1) / (float)TEX_HEIGHT;
  add_vertex(
      v, x, z, y,
      DrawableSection::tex_arr_x[0][rotation % 4],
      DrawableSection::tex_arr_y[0][rotation % 4] + tex_offset);
  add_vertex(
      v, x, z, y + sz,
      DrawableSection::tex_arr_x[0][(rotation + 1) % 4],
      DrawableSection::tex_arr_y[0][(rotation + 1) % 4] + tex_offset);
  add_vertex(
      v, x + sz, z, y + sz,
      DrawableSection::tex_arr_x[0][(rotation + 2) % 4],
      DrawableSection::tex_arr_y[0][(rotation + 2) % 4] + tex_offset);
  add_vertex(
      v, x + sz, z, y,
      DrawableSection::tex_arr_x[0][(rotation + 3) % 4],
      DrawableSection::tex_arr_y[0][(rotation + 3) % 4] + tex_offset);
  
  add_triangle(
      curr_ind, curr_ind + 1, curr_ind + 2);
  add_triangle(
      curr_ind + 2, curr_ind + 3, curr_ind);

}

void DrawableSection::add_left_right(
    std::vector<float> &v, 
    std::vector<unsigned char> &ind,
    float x, float y, float z,
    float sz, 
    char tex_type, short tex_row,
    char rotation) {
  auto curr_ind = v.size();
  short tex_offset = (TEX_HEIGHT - tex_row - 1) / (float)TEX_HEIGHT;
  add_vertex(
      v, x + sz, z, y,
      DrawableSection::tex_arr_x[0][rotation % 4],
      DrawableSection::tex_arr_y[0][rotation % 4] + tex_offset);
  add_vertex(
      v, x + sz, z + sz, y,
      DrawableSection::tex_arr_x[0][(rotation + 1) % 4],
      DrawableSection::tex_arr_y[0][(rotation + 1) % 4] + tex_offset);
  add_vertex(
      v, x, z + sz, y,
      DrawableSection::tex_arr_x[0][(rotation + 2) % 4],
      DrawableSection::tex_arr_y[0][(rotation + 2) % 4] + tex_offset);
  add_vertex(
      v, x, z, y,
      DrawableSection::tex_arr_x[0][(rotation + 3) % 4],
      DrawableSection::tex_arr_y[0][(rotation + 3) % 4] + tex_offset);
  
  add_triangle(
      curr_ind, curr_ind + 1, curr_ind + 2);
  add_triangle(
      curr_ind + 2, curr_ind + 3, curr_ind);

}

void DrawableSection::add_front_back(
    std::vector<float> &v, 
    std::vector<unsigned char> &ind,
    float x, float y, float z,
    float sz, 
    char tex_type, short tex_row,
    char rotation) {
  auto curr_ind = v.size();
  short tex_offset = (TEX_HEIGHT - tex_row - 1) / (float)TEX_HEIGHT;
  add_vertex(
      v, x, z, y,
      DrawableSection::tex_arr_x[0][rotation % 4],
      DrawableSection::tex_arr_y[0][rotation % 4] + tex_offset);
  add_vertex(
      v, x, z + sz, y,
      DrawableSection::tex_arr_x[0][(rotation + 1) % 4],
      DrawableSection::tex_arr_y[0][(rotation + 1) % 4] + tex_offset);
  add_vertex(
      v, x, z + sz, y + sz,
      DrawableSection::tex_arr_x[0][(rotation + 2) % 4],
      DrawableSection::tex_arr_y[0][(rotation + 2) % 4] + tex_offset);
  add_vertex(
      v, x, z, y + sz,
      DrawableSection::tex_arr_x[0][(rotation + 3) % 4],
      DrawableSection::tex_arr_y[0][(rotation + 3) % 4] + tex_offset);
  
  add_triangle(
      curr_ind, curr_ind + 1, curr_ind + 2);
  add_triangle(
      curr_ind + 2, curr_ind + 3, curr_ind);

}


DrawableSection::DrawableSection(
    BaseSection<BaseBlock> *section) {
  if (section->isContiguous()) return;

  if (!DrawableSection::static_init) {
    float tex_mult = 1.0f / (float)TEX_WIDTH;
    for (int i = 0; i < TEX_WIDTH; i ++) {
      // Bottom left
      DrawableSection::tex_arr_x[i][0] = 0.0f + i * tex_mult;
      DrawableSection::tex_arr_y[i][0] = 0.0f;
    
      // Top left
      DrawableSection::tex_arr_x[i][1] = 0.0f + i * tex_mult;
      DrawableSection::tex_arr_y[i][1] = 1.0f / (float)TEX_HEIGHT;

      // Top right
      DrawableSection::tex_arr_x[i][2] = (i + 1) * tex_mult;
      DrawableSection::tex_arr_y[i][2] = 1.0f / (float)TEX_HEIGHT;

      // Bottom right
      DrawableSection::tex_arr_x[i][3] = (i + 1) * tex_mult;
      DrawableSection::tex_arr_y[i][3] = 0.0f;
    }
    DrawableSection::static_init = true;
  }

  std::vector<float> vertices;
  std::vector<unsigned char> indices;

  for (int i = 0; i < section.get_edge(); i ++) {
    for (int j = 0; j < section.get_edge(); j ++) {
      for (int k = 0; k < section.get_edge(); k ++) {
        cuboc::BaseBlock block = section.get(i, j, k);
        if (block.getType() == BLOCK_AIR) continue;
        short tex_row = block.getTexRow();

        float i_f = (float) i / BLOCK_SIZE;
        float j_f = (float) j / BLOCK_SIZE;
        float k_f = (float) k / BLOCK_SIZE;
        
        add_bottom_top(
            vertices, indices,
            i_f, j_f, k_f, 
            BLOCK_SIZE,
            block.getTexType(), block.getTexRow(),
            0);
        add_bottom_top(
            vertices, indices,
            i_f, j_f, k_f + BLOCK_SIZE, 
            BLOCK_SIZE,
            block.getTexType(), block.getTexRow(),
            2);

        add_left_right(
            vertices, indices,
            i_f, j_f, k_f, 
            BLOCK_SIZE,
            block.getTexType(), block.getTexRow(),
            0);
        add_left_right(
            vertices, indices,
            i_f, j_f + BLOCK_SIZE, k_f, 
            BLOCK_SIZE,
            block.getTexType(), block.getTexRow(),
            2);

        add_front_back(
            vertices, indices,
            i_f, j_f, k_f, 
            BLOCK_SIZE,
            block.getTexType(), block.getTexRow(),
            0);
        add_front_back(
            vertices, indices,
            i_f + BLOCK_SIZE, j_f, k_f, 
            BLOCK_SIZE,
            block.getTexType(), block.getTexRow(),
            2);
      }
    }
  }
}

void DrawableSection::add_vertex(
    std::vector<float> &vertices,
    float px,
    float py,
    float pz,
    float ytex,
    float xtex) {

  vertices.push_back(px);
  vertices.push_back(py);
  vertices.push_back(pz);
  vertices.push_back(xtex);
  vertices.push_back(ytex);
}

void DrawableSection::add_triangle(
    std::vector<unsigned char> &inds,
    unsigned char ind1,
    unsigned char ind2,
    unsigned char ind3) {
  inds.push_back(ind1);
  inds.push_back(ind2);
  inds.push_back(ind3);
}

World::World(
    graphicsutils::ProgramLoader *program,
    TextureFactory *atlas) :
    _program(program),
    _tex_atlas(atlas) {}

}
