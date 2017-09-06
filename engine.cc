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

GLuint VAO[2];
GLuint VBO[2];
GLuint EBO[2];

unsigned int texture1;
unsigned int texture2;

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

  glGenBuffers(2, EBO);
  glGenBuffers(2, VBO);  
  glGenVertexArrays(2, VAO);


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


  // -------------------- SETUP TEXTURES -----------------------
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  Loader::Image container = 
      Loader::Image("textures/container.jpg", 3);
  Loader::Image brick_wall = 
      Loader::Image("textures/brick_wall.jpg", 3);
 
  void *data = (void *)brick_wall.getBuffer();
  glTexImage2D(GL_TEXTURE_2D, 0, 
      GL_RGB, 
      brick_wall.getWidth(), 
      brick_wall.getHeight(), 
      0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  data = (void *)container.getBuffer();
  glTexImage2D(GL_TEXTURE_2D, 0, 
      GL_RGB, 
      container.getWidth(), 
      container.getHeight(), 
      0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

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

  base::Window window = base::Window(800, 600, "Window Fun");
  glfwSetErrorCallback(err_callback);

  initGL(window);

  programs[0].use();
  programs[0].setInt("texture1", 0);
  programs[0].setInt("texture2", 1);
  
  
  std::printf("Running\n");
  while (!glfwWindowShouldClose(window.getWindow())) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    GLfloat time = glfwGetTime();
    int index = (int) (sin(time) / 2 + 1.0);   
    programs[index].use();
    if (index == 0) {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture1);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, texture2);
     
      glm::mat4 model;
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
    glfwSwapBuffers(window.getWindow());
    glfwPollEvents();
  }

  
  glDeleteVertexArrays(2, VAO);
  glDeleteBuffers(2, VBO);
  glDeleteBuffers(2, EBO);
  glfwTerminate();
  
  std::printf("Terminating\n");
  return 0;
}
