#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "window.h"
#include "loader/programloader.h"
#include "loader/buffer.h"
#include "loader/image_loader.h"

#define PI 3.14159265

std::vector<graphicsutils::ProgramLoader> programs;

GLuint VAO[1];
GLuint VBO[1];
GLuint EBO[1];

unsigned int texture1;

glm::mat4 model;

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

void initGL(base::Window window) {

  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    std::fprintf(stderr,"Fatal error initializing GLEW. Aborting.\n");
    std::exit(EXIT_FAILURE);
  }

  glfwSetKeyCallback(window.getWindow(), key_callback);
 
  glViewport(0, 0, window.getWidth(), window.getHeight());

  // ---------------- SETUP PROGRAMS -----------------
  programs.push_back(
      graphicsutils::ProgramLoader(
          "shaders/vertex.glsl",
          "shaders/fragment.glsl"));
  programs.push_back(
      graphicsutils::ProgramLoader(
          "shaders/vertex_color.glsl",
          "shaders/fragment_color.glsl"));

  // ---------------- INITIALIZE BUFFERS --------------

  glGenBuffers(1, EBO);
  glGenBuffers(1, VBO);  
  glGenVertexArrays(1, VAO);

  float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  float texCoords[] = {
      0.0f, 0.0f,  // lower-left corner  
      1.0f, 0.0f,  // lower-right corner
      0.5f, 1.0f   // top-center corner
  };


  // -------------------- SETUP TEXTURES -----------------------
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  Loader::Image container = 
      Loader::Image("textures/container.jpg", 3);
   
  void *data = (void *)container.getBuffer();
  glTexImage2D(GL_TEXTURE_2D, 0, 
      GL_RGB, 
      container.getWidth(), 
      container.getHeight(), 
      0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  programs[0].use();
  programs[0].setInt("texture1", 0);

  // --------------- Transformation matrices -----------------
  model = glm::rotate(
      model,
      glm::radians(-55.0f),
      glm::vec3(1.0f, 0.0f, 0.0f));
  glm::mat4 view;
  view = glm::translate(
      view,
      glm::vec3(0.0f, 0.0f, -3.0f));
  glm::mat4 proj = glm::perspective(
      glm::radians(45.0f),
      (float)window.getWidth()/(float)window.getHeight(),
      0.1f,
      100.0f);
  
  programs[0].setMatrix("projection", proj);
  programs[0].setMatrix("view", view);
  programs[0].setMatrix("model", model);

  // ---------------- GENERATE RECTANGLE -----------------
  glBindVertexArray(VAO[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
      5 * sizeof(float),
      (GLvoid*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
      5 * sizeof(float),
      (GLvoid*)(3 * sizeof(float)));
  
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

}

int main(int argc, char** argv) {

  base::Window window = base::Window(800, 600, "Window Fun");
  glfwSetErrorCallback(err_callback);

  initGL(window);
  
  
  std::printf("Running\n");
  while (!glfwWindowShouldClose(window.getWindow())) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    GLfloat time = glfwGetTime();
    programs[0].use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    model = glm::rotate(
        model, 
        glm::radians(0.02f), 
        glm::vec3(1.0f, 0.3f, 0.5f));
    programs[0].setMatrix("model", model);

    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0);
    glUseProgram(0);

    glfwSwapInterval(1);
    glfwSwapBuffers(window.getWindow());
    glfwPollEvents();
  }

  
  glDeleteVertexArrays(1, VAO);
  glDeleteBuffers(1, VBO);
  glDeleteBuffers(1, EBO);
  glfwTerminate();
  
  std::printf("Terminating\n");
  return 0;
}
