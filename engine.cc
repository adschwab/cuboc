#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <cmath>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <GLFW/glfw3.h>

#include "window.h"
#include "loader/programloader.h"
#include "loader/image_loader.h"

#define PI 3.14159265

int target_fps = 30;
GLfloat loop_time = 1/(float)target_fps;

std::vector<graphicsutils::ProgramLoader> programs;

GLuint VAO[1];
GLuint VBO[1];
GLuint EBO[1];

unsigned int texture1;

base::Window window = base::Window(800, 600, "Window Fun");

double mouseX = window.getWidth()/2;
double mouseY = window.getHeight()/2;
bool mouse_set = false;
float sensitivity = 5;

glm::mat4 model;
glm::mat4 view;
glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);

glm::vec3 cam_pos = glm::vec3(0.0f, 1.0f, 2.0f);
glm::vec3 origin_angle = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
glm::vec3 cam_angle = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));

float h_angle;
float v_angle;


float h_theta(glm::vec3 origin, glm::vec3 dir) {
  float cos_angle = glm::dot(dir, origin);
  float mult = 1.0;
  if (dir[0] < 0)
    mult = -1.0;
  return glm::acos(cos_angle) * mult;
}

static void err_callback(
    int error,
    const char* description) {
#ifndef RELEASE  
  std::fprintf(stderr, "Error: %s\n", description);
#endif
}

glm::quat rotation(glm::vec3 start, glm::vec3 dest){

  float cosTheta = glm::dot(start, dest);
  glm::vec3 rotationAxis;
  if (cosTheta < -1 + 0.00001f){
    rotationAxis = glm::cross(
        glm::vec3(0.0f, 0.0f, 1.0f), start);
    if (length2(rotationAxis) < 0.00001 )
      rotationAxis = glm::cross(
          glm::vec3(1.0f, 0.0f, 0.0f), start);
    rotationAxis = glm::normalize(rotationAxis);
    return glm::angleAxis(
        glm::radians(180.0f), rotationAxis);
  }
  rotationAxis = glm::cross(start, dest);
  float s = glm::sqrt( (1+cosTheta)*2 );
  float invs = 1 / s;
  return glm::quat(
      s * 0.5f, 
      rotationAxis.x * invs,
      rotationAxis.y * invs,
      rotationAxis.z * invs);
}

void update_view() {
  view = glm::mat4();
  glm::vec3 pos = cam_pos;
  pos[1] = -pos[1];
  
  view = glm::translate(view, pos);
  float xang = glm::sin(-h_angle);
  float yang = glm::cos(-h_angle);
  glm::vec3 angle = glm::vec3(xang, 0.0f, yang);
  view = glm::toMat4(rotation(origin_angle, angle)) * view;
  
  yang = glm::cos(v_angle);
  float zang = glm::sin(v_angle);
  angle = glm::vec3(0.0f, zang, yang);
  view = glm::toMat4(rotation(
      glm::vec3(0.0f, 0.0f, 1.0f), angle)) * view;

}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
    cam_pos[2] -= 0.1f * glm::cos(h_angle);
    cam_pos[0] -= 0.1f * glm::sin(h_angle);
    update_view();
  }
  else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
    cam_pos[2] += 0.1f * glm::cos(h_angle);
    cam_pos[0] += 0.1f * glm::sin(h_angle);
    update_view();
  }
  else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
    cam_pos[2] += 0.1f * glm::sin(h_angle);
    cam_pos[0] -= 0.1f * glm::cos(h_angle);
    update_view();
  }
  else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
    cam_pos[2] -= 0.1f * glm::sin(h_angle);
    cam_pos[0] += 0.1f * glm::cos(h_angle);
    update_view();
  }
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

  double norm = std::sqrt(xdiff * xdiff + ydiff * ydiff);
  h_angle = h_angle - xdiff * PI/(180.0f * sensitivity);
  v_angle = glm::max(
      glm::min(
        v_angle - ydiff * PI/(180.0f * sensitivity),
        PI/2.0f),
      -PI/2.0f);
  update_view();
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
  
  model = glm::translate(model, -position);
  
  model = glm::rotate(
      model,
      glm::radians(0.0f),
      glm::vec3(1.0f, 0.0f, 0.0f));
  
  h_angle = h_theta(origin_angle, cam_angle); 
  v_angle = -0.3f;
  update_view();
  
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
  glfwSetErrorCallback(err_callback);
  glfwSetKeyCallback(window.getWindow(), key_callback);
  glfwSetCursorPosCallback(window.getWindow(), cursor_pos_callback);
  glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  initGL(window);
  GLfloat lap_time = glfwGetTime();
  
  std::printf("Running\n");
  while (!glfwWindowShouldClose(window.getWindow())) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    programs[0].use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    programs[0].setMatrix("view", view);
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0);
    glUseProgram(0);
    
    GLfloat time = glfwGetTime();
    GLfloat render_time = time - lap_time;
    lap_time = time; 

    if (render_time > loop_time) {
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
