#ifndef WINDOW_H
#define WINDOW_H

#include <string>

#include <GLFW/glfw3.h>

namespace base {

class Window {
  public:
  Window(int width, int height, std::string title);
  
  int getWidth();
  int getHeight();
  GLFWwindow* getWindow();
  bool getFocus();
  void setFocus(bool focus);

  private:
  GLFWwindow* _window;
  std::string _title;
  int _width;
  int _height;
  bool has_focus;
};

} // namespace
#endif
