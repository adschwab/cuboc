#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "window.h"
#include "camera.h"
#include "movement.h"
#include "loader/programloader.h"
#include "loader/texture_loader.h"
#include "text.h"
#include "world.h"
#include "util/hashkey.h"
#include "util/log.h"

int target_fps = 30;
GLfloat loop_time = 1/(float)target_fps;

std::vector<std::shared_ptr<graphicsutils::ProgramLoader> >
    programs;

cuboc::World *world;

base::Window window(1000, 600, "Window Fun");
TextureFactory tex_factory("textures");

double mouseX = window.getWidth()/2;
double mouseY = window.getHeight()/2;
bool mouse_set = false;

glm::vec3 cam_pos = glm::vec3(0.0f, 0.5f, 0.0f);
XYZCoord cam_ind(0, 0, 0);

cuboc::Camera camera(&window, cam_pos, cam_ind, 3.14f/2.0f, 0.0f);
cuboc::Movement movement(&camera);

glm::vec3 fogColor(197.0f/256.0f, 210.0f/256.0f, 209.0f/256.0f);


static void err_callback(
    int error,
    const char* description) {
#ifndef RELEASE  
  std::fprintf(stderr, "Error: %s\n", description);
#endif
}

static void focus_callback(GLFWwindow* _window, int focused) {
  window.setFocus((bool)focused);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    movement.setBackward(true);
  else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
    movement.setBackward(false);
  else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    movement.setForward(true);
  else if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
    movement.setForward(false);
  else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    movement.setRight(true);
  else if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
    movement.setRight(false);
  else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    movement.setLeft(true);
  else if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
    movement.setLeft(false); 

}

static void cursor_pos_callback(GLFWwindow* _window, 
    double xpos,
    double ypos) {
  double xdiff = xpos - mouseX;
  double ydiff = ypos - mouseY;
  
  mouseX = xpos;
  mouseY = ypos;
  if (!mouse_set) {
    mouse_set = true;
    return;
  }
  
  camera.updateAngles(xdiff, ydiff);
}

void initGL() {

  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    std::fprintf(stderr,"Fatal error initializing GLEW. Aborting.\n");
    std::exit(EXIT_FAILURE);
  }

  // ---------------- SETUP PROGRAMS -----------------
  std::shared_ptr<graphicsutils::Program3d> program =
      std::make_shared<graphicsutils::Program3d> (
          "shaders/vertex.glsl",
          "shaders/fragment.glsl",
          &camera);
  program->use();
  program->setVec("fogColor", fogColor);
  programs.push_back(program);
  
  std::shared_ptr<graphicsutils::ProgramLoader> img_program =
      std::make_shared<graphicsutils::ProgramLoader> (
          "shaders/vertex_img.glsl",
          "shaders/fragment_img.glsl");
  programs.push_back(img_program);
  // ---------------- GENERATE RECTANGLE -----------------
  world = new cuboc::World(static_cast<graphicsutils::Program3d *>(programs[0].get()), tex_factory.get("grounds_atlas"), &camera);
  //box = new Box(programs[0].get(), &tex_factory);

}

int main(int argc, char** argv) {
  glfwSetWindowFocusCallback(window.getWindow(), focus_callback);
  glfwSetErrorCallback(err_callback);
  glfwSetKeyCallback(window.getWindow(), key_callback);
  glfwSetCursorPosCallback(window.getWindow(), cursor_pos_callback);
  glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetKeyCallback(window.getWindow(), key_callback);

  initGL();
  GLfloat lap_time = glfwGetTime();
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  DrawText text(programs[1].get());

  std::printf("Running\n");
  while (window.run()) {
    movement.updatePosition(world->get_raw());
    glClearColor(fogColor[0], fogColor[1], fogColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    world->draw();
    
    //text.draw("HELLO WORLD", -0.9f, 0.9f, 0.1f, window.aspectRatio());
    graphicsutils::ProgramLoader::done();
    
    GLfloat time = glfwGetTime();
    GLfloat render_time = time - lap_time;
    lap_time = time;

    if (window.getFocus() && render_time > loop_time) {
      std::printf("Render took too long: %.3f\n",
          render_time);
    }
    glfwSwapInterval(1);
    glfwSwapBuffers(window.getWindow());
    glfwPollEvents();
  }
  
  glfwTerminate();
  
  std::printf("Terminating\n");
  return 0;
}
