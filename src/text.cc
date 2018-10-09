
#include "text.h"
#include "loader/texture_loader.h"

DrawText::DrawText(graphicsutils::ProgramLoader *img_program) :
    img_program_(img_program),
    text_img_("img/text2.png", false) {}

void DrawText::draw(std::string str, float left, float top, float pt, float aspectRatio) {
  float offset = 1.0f / 8.0f;

  if (str.size() == 0) {
    return;
  }

  int max_size = str.size() > 256 / 4 ? 256 / 4 : str.size();
  std::vector<float> vertices;
  std::vector<unsigned char> indices;
  
  for (int i = 0; i < max_size; i ++) {
    int id = str[i] - 32;

    float xcell = id % 8;
    float ycell = id / 8;
    float xtex = xcell * offset;
    float ytex = ycell * offset;

    float ptx = pt;
    float pty = pt * aspectRatio;
    float vlist[] = {
        left, top - pty, xtex, ytex + offset,
        left + ptx, top - pty, xtex + offset, ytex + offset,
        left + ptx, top, xtex + offset, ytex,
        left, top, xtex, ytex
    };
    vertices.insert(vertices.end(), vlist, vlist + 16);
  
    unsigned char indlist[] = {
        static_cast<unsigned char>(i * 4),
        static_cast<unsigned char>(i * 4 + 1),
        static_cast<unsigned char>(i * 4 + 2),
        static_cast<unsigned char>(i * 4),
        static_cast<unsigned char>(i * 4 + 2),
        static_cast<unsigned char>(i * 4 + 3)
    };
    indices.insert(indices.end(), indlist, indlist + 6);
    left = left + ptx;
  }

  std::vector<glw::BufferAttr> attr_info;
  glw::BufferAttr vertex;
  vertex.num = 2;
  attr_info.push_back(vertex);
  glw::BufferAttr texture;
  texture.num = 2;
  attr_info.push_back(texture);
  
  text_img_.set(img_program_); 

  glw::VertexBuffer v_buff(vertices);
  glw::IndexBuffer i_buff(indices);
  glw::ArrayObject ao(v_buff,i_buff, attr_info);
  ao.draw();
  graphicsutils::ProgramLoader::done();
}



