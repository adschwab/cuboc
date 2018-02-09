#include "drawable_section.h"

#include <cstdio>
#include <vector>

#include "glwrapper/buffer.h"

namespace cuboc {

bool DrawableSection::static_init = false;
float DrawableSection::tex_arr_x[TEX_WIDTH][4];
float DrawableSection::tex_arr_y[TEX_WIDTH][4];

void DrawableSection::draw(glm::vec3 offset) {
  ao_->draw();
}

void DrawableSection::add_bottom_top(
    std::vector<float> &v, 
    std::vector<unsigned char> &ind,
    float x, float y, float z,
    float sz, 
    char tex_type, short tex_row,
    char rotation) {
  auto curr_ind = v.size() / 5;
  float tex_offset = tex_row / (float)TEX_HEIGHT;
  char tex_ind = tex_type == TEX_ONE ? 0 : 1;
  add_vertex(
      v, x, z, y,
      DrawableSection::tex_arr_x[tex_ind][rotation % 4],
      DrawableSection::tex_arr_y[tex_ind][rotation % 4] + tex_offset);
  add_vertex(
      v, x, z, y + sz,
      DrawableSection::tex_arr_x[tex_ind][(rotation + 1) % 4],
      DrawableSection::tex_arr_y[tex_ind][(rotation + 1) % 4] + tex_offset);
  add_vertex(
      v, x + sz, z, y + sz,
      DrawableSection::tex_arr_x[tex_ind][(rotation + 2) % 4],
      DrawableSection::tex_arr_y[tex_ind][(rotation + 2) % 4] + tex_offset);
  add_vertex(
      v, x + sz, z, y,
      DrawableSection::tex_arr_x[tex_ind][(rotation + 3) % 4],
      DrawableSection::tex_arr_y[tex_ind][(rotation + 3) % 4] + tex_offset);
  
  add_triangle(
      ind, curr_ind, curr_ind + 1, curr_ind + 2);
  add_triangle(
      ind, curr_ind + 2, curr_ind + 3, curr_ind);

}

void DrawableSection::add_left_right(
    std::vector<float> &v, 
    std::vector<unsigned char> &ind,
    float x, float y, float z,
    float sz, 
    char tex_type, short tex_row,
    char rotation) {
  auto curr_ind = v.size() / 5;
  float tex_offset = tex_row / (float)TEX_HEIGHT;

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
      ind, curr_ind, curr_ind + 1, curr_ind + 2);
  add_triangle(
      ind, curr_ind + 2, curr_ind + 3, curr_ind);

}

void DrawableSection::add_front_back(
    std::vector<float> &v, 
    std::vector<unsigned char> &ind,
    float x, float y, float z,
    float sz, 
    char tex_type, short tex_row,
    char rotation) {
  auto curr_ind = v.size() / 5;
  float tex_offset = tex_row / (float)TEX_HEIGHT;
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
      ind, curr_ind, curr_ind + 1, curr_ind + 2);
  add_triangle(
      ind, curr_ind + 2, curr_ind + 3, curr_ind);

}


DrawableSection::DrawableSection(
    BaseSection<Block> *section, float size) {
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

  for (int i = 0; i < section->get_edge(); i ++) {
    for (int j = 0; j < section->get_edge(); j ++) {
      for (int k = 0; k < section->get_edge(); k ++) {
        cuboc::Block block = section->get(i, j, k);
        if (block.getType() == BLOCK_AIR) continue;
        short tex_row = block.getTexRow();

        float i_f = (float) i / size;
        float j_f = (float) j / size;
        float k_f = (float) k / size;
        
        add_bottom_top(
            vertices, indices,
            i_f, j_f, k_f, 
            size,
            block.getTexType(), block.getTexRow(),
            0);
        add_bottom_top(
            vertices, indices,
            i_f, j_f, k_f + size, 
            size,
            block.getTexType(), block.getTexRow(),
            2);

        add_left_right(
            vertices, indices,
            i_f, j_f, k_f, 
            size,
            block.getTexType(), block.getTexRow(),
            0);
        add_left_right(
            vertices, indices,
            i_f, j_f + size, k_f, 
            size,
            block.getTexType(), block.getTexRow(),
            2);

        add_front_back(
            vertices, indices,
            i_f, j_f, k_f, 
            size,
            block.getTexType(), block.getTexRow(),
            0);
        add_front_back(
            vertices, indices,
            i_f + size, j_f, k_f, 
            size,
            block.getTexType(), block.getTexRow(),
            2);
      }
    }
  }

  std::vector<glw::BufferAttr> attr_info;

  glw::BufferAttr vertex;
  vertex.num = 3;
  attr_info.push_back(vertex);
  glw::BufferAttr texture;
  texture.num = 2;
  attr_info.push_back(texture);

  glw::VertexBuffer v_buff(vertices);
  glw::IndexBuffer i_buff(indices);
  ao_ = std::make_shared<glw::ArrayObject>(
      v_buff,i_buff, attr_info);
}

void DrawableSection::add_vertex(
    std::vector<float> &vertices,
    float px,
    float py,
    float pz,
    float xtex,
    float ytex) {
  std::printf("%f, %f, %f, %f, %f\n", px, py, pz, xtex, ytex);
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
  std::printf("%d, %d, %d\n", (int)ind1, (int)ind2, (int)ind3);  
  inds.push_back(ind1);
  inds.push_back(ind2);
  inds.push_back(ind3);
}


}
