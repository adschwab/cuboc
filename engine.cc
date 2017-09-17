#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <cmath>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "window.h"
#include "camera.h"
#include "movement.h"
#include "loader/programloader.h"
#include "loader/image_loader.h"


int target_fps = 30;
GLfloat loop_time = 1/(float)target_fps;

std::vector<graphicsutils::ProgramLoader> programs;

GLuint VAO[1];
GLuint VBO[1];
GLuint EBO[1];

unsigned int texture1;

base::Window window = base::Window(1000, 600, "Window Fun");


double mouseX = window.getWidth()/2;
double mouseY = window.getHeight()/2;
bool mouse_set = false;
float sensitivity = 5;

glm::vec3 cam_pos = glm::vec3(0.0f, 2.0f, 0.0f);
Camera camera = Camera(cam_pos, 0.0f, 0.0f);
Movement movement = Movement(&camera);

glm::vec3 positions[] = {
    glm::vec3(0.0f, 0.0f, 3.0f),
    glm::vec3(0.0f, 0.0f, 4.0f),
    glm::vec3(0.0f, 0.0f, 5.0f),
    glm::vec3(0.0f, 1.0f, 3.0f),
    glm::vec3(1.0f, 0.0f, 3.0f),
    glm::vec3(1.0f, 0.0f, 4.0f),
    glm::vec3(1.0f, 0.0f, 5.0f)
};

int num_positions = 7;

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
  
  camera.updateAngles(xdiff, ydiff, sensitivity);
}

void initGL(base::Window window) {

  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    std::fprintf(stderr,"Fatal error initializing GLEW. Aborting.\n");
    std::exit(EXIT_FAILURE);
  }

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


  // -------------------- SETUP TEXTURES -----------------------
  glGenTextures(1, &texture1);
  glActiveTexture(GL_TEXTURE1);
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


  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, texture1);
  programs[0].use();
  programs[0].setInt("texture1", 2);


  // --------------- Transformation matrices -----------------
  
  glm::mat4 proj = glm::perspective(
      glm::radians(45.0f),
      (float)window.getWidth()/(float)window.getHeight(),
      0.1f,
      100.0f);
  
  programs[0].setMatrix("projection", proj);
  programs[0].setMatrix("view", camera.getView());

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
  glfwSetWindowFocusCallback(window.getWindow(), focus_callback);
  glfwSetErrorCallback(err_callback);
  glfwSetKeyCallback(window.getWindow(), key_callback);
  glfwSetCursorPosCallback(window.getWindow(), cursor_pos_callback);
  glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetKeyCallback(window.getWindow(), key_callback);
  glfwWindowHint(GLFW_SAMPLES, 8);

  initGL(window);
  GLfloat lap_time = glfwGetTime();
  glEnable(GL_MULTISAMPLE);

  std::printf("Running\n");
  while (!glfwWindowShouldClose(window.getWindow())) {
    movement.updatePosition();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    programs[0].use();

    programs[0].setMatrix("view", camera.getView());
    glBindVertexArray(VAO[0]);
    for (int i = 0; i < num_positions; i ++) {
      glm::mat4 model;
      glm::vec3 position = positions[i];
      position[1] = -position[1]; 
      model = glm::translate(model, -position);
      programs[0].setMatrix("model", model);  
      glDrawArrays(GL_TRIANGLES, 0, 36);
    } 

    glBindVertexArray(0);
    glUseProgram(0);
    
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

  
  glDeleteVertexArrays(1, VAO);
  glDeleteBuffers(1, VBO);
  glDeleteBuffers(1, EBO);
  glfwTerminate();
  
  std::printf("Terminating\n");
  return 0;
}
