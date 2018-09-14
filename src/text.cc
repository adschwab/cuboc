
#include "text.h"
#include "loader/texture_loader.h"

DrawText::DrawText(graphicsutils::ProgramLoader *img_program) :
    img_program_(img_program),
    text_img_("img/text.bmp", false) {}

void DrawText::draw(std::string str, float left, float top, float pt) {
  text_img_.set(img_program_); 
  
  std::vector<float> vertices = {
      left, top, 0.0f, 1.0f,
      left + pt, top, 1.0f, 1.0f,
      left + pt, top + pt, 1.0f, 0.0f,
      left, top + pt, 0.0f, 0.0f};
  
  std::vector<unsigned char> indices = {
      0, 1, 2,
      0, 2, 3
  };

  std::vector<glw::BufferAttr> attr_info;
  glw::BufferAttr vertex;
  vertex.num = 2;
  attr_info.push_back(vertex);
  glw::BufferAttr texture;
  texture.num = 2;
  attr_info.push_back(texture);

  glw::VertexBuffer v_buff(vertices);
  glw::IndexBuffer i_buff(indices);
  glw::ArrayObject ao(v_buff,i_buff, attr_info);
  ao.draw();
  graphicsutils::ProgramLoader::done();
}



