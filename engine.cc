#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "loader/programloader.h"
#include "loader/buffer.h"
#include "loader/image_loader.h"

#define PI 3.14159265

std::vector<graphicsutils::ProgramLoader> programs;

GLuint VAO[2];
GLuint VBO[2];
GLuint EBO[2];

unsigned int texture;

static void err_callback(int error, const char* description) {
#ifndef RELEASE  
  std::fprintf(stderr, "Error: %s\n", description);
#endif
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void initGL(GLFWwindow* window) {

  glfwMakeContextCurrent(window);

  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    std::fprintf(stderr,"Fatal error initializing GLEW. Aborting.\n");
    std::exit(EXIT_FAILURE);
  }

  glfwSetKeyCallback(window, key_callback);
 
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  GLfloat vertices[] = {
      0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
      0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
     -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 
     -0.5f,  0.5f, 0.0f,  0.0f, 1.0f 
  };

  GLfloat vertices2[] = {
     -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
      0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
  };

  GLuint indices[] = {
      0, 1, 3,
      1, 2, 3
  };

  float texCoords[] = {
      0.0f, 0.0f,  // lower-left corner  
      1.0f, 0.0f,  // lower-right corner
      0.5f, 1.0f   // top-center corner
  };

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  Loader::Image brick_wall = 
      Loader::Image("textures/brick_wall.jpg", 3);
 
  void *data = (void *)brick_wall.getBuffer();
  glTexImage2D(GL_TEXTURE_2D, 0, 
      GL_RGB, 
      brick_wall.getWidth(), 
      brick_wall.getHeight(), 
      0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  programs.push_back(
      graphicsutils::ProgramLoader(
          "shaders/vertex.glsl",
          "shaders/fragment.glsl"));
  programs.push_back(
      graphicsutils::ProgramLoader(
          "shaders/vertex_color.glsl",
          "shaders/fragment_color.glsl"));

  glGenBuffers(2, EBO);
  glGenBuffers(2, VBO);  
  glGenVertexArrays(2, VAO);

  // ---------------- GENERATE RECTANGLE -----------------
  glBindVertexArray(VAO[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
      5 * sizeof(float),
      (GLvoid*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
      5 * sizeof(float),
      (GLvoid*)(3 * sizeof(float)));
  
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);


  // --------------- GENERATE TRIANGLE -------------------
  glBindVertexArray(VAO[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)0);  
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*) (3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

}

int main(int argc, char** argv) {

  GLFWwindow* window;
  glfwSetErrorCallback(err_callback);
  if (!glfwInit()) {
    std::fprintf(stderr, "Fatal Error initializing glfw. Aborting.\n");
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* currentMode = glfwGetVideoMode(monitor);
  std::printf("width: %d\nheight: %d\nfps: %d\n", currentMode->width, currentMode->height, currentMode->refreshRate);
  
  window = glfwCreateWindow(800, 600, "Window fun", NULL, NULL);
  if (!window) {
    std::fprintf(stderr, "Fatal Error initializing monitor. Aborting.\n");
    exit(EXIT_FAILURE);
  }
  
  initGL(window);

  
  
  std::printf("Running\n");
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    GLfloat time = glfwGetTime();
    int index = (int) (sin(time) / 2 + 1.0);   
    programs[index].use();
    if (index == 0) {
      GLfloat color1 = (sin(time) / 2) + 0.5;
      GLfloat color2 = (sin(time + PI) / 2) + 0.5;

      programs[index].setFloat("incolor1", color1);
      programs[index].setFloat("incolor2", color2);

      glBindVertexArray(VAO[index]);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    else {
      glBindVertexArray(VAO[index]);
      glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    glBindVertexArray(0);
    glUseProgram(0);

    glfwSwapInterval(1);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  
  glDeleteVertexArrays(2, VAO);
  glDeleteBuffers(2, VBO);
  glDeleteBuffers(2, EBO);
  glfwTerminate();
  
  std::printf("Terminating\n");
  return 0;
}
