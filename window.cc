#include "window.h"

#include <string>
#include <cstdlib>

#include <GLFW/glfw3.h>

namespace base {

  Window::Window(int width, int height, std::string title):
      _width(width), 
      _height(height),
      _title(title),
      has_focus(true){

    if (!glfwInit()) {
      std::fprintf(
          stderr,
          "Fatal Error initializing glfw. Aborting.\n");
      exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(
        GLFW_OPENGL_PROFILE, 
        GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CURSOR_DISABLED, GL_TRUE);
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* currentMode = glfwGetVideoMode(
        monitor);
    std::printf(
        "Screen Width: %d\nScreen Height: %d\nfps: %d\n", 
        currentMode->width,
        currentMode->height, 
        currentMode->refreshRate);

    _window = glfwCreateWindow(
        _width,
        _height,
        _title.c_str(),
        NULL, NULL);
    
    if (!_window) {
      std::fprintf(
          stderr,
          "Fatal Error initializing monitor. Aborting.\n");
      exit(1);
    }
    
    glfwMakeContextCurrent(_window);
    glfwGetFramebufferSize(_window, &_width, &_height);
    glViewport(0, 0, _width, _height);
  }

  bool Window::run() {
    return !glfwWindowShouldClose(_window);
  }

  bool Window::getFocus() {
    return has_focus;
  }

  void Window::setFocus(bool focus) {
    has_focus = focus;
  }

  GLFWwindow *Window::getWindow() {
    return _window;
  }

  int Window::getWidth() {
    return _width;
  }

  int Window::getHeight() {
    return _height;
  }
} // namespace
