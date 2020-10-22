#pragma once

#include "gl.h"
#include "ecs.h"
#include "input.h"
#include "constants.h"
#include "glm.hpp"
#include "gtx/string_cast.hpp"
#include "gtc/matrix_transform.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Lucid {
 public:
  Registry* registry;
  Input* input;
  GLFWwindow* window;

  bool keys[350];

  uint32_t VAO, VBO, EBO;
  uint32_t vertexShader, fragmentShader;
  uint32_t shaderProgram;

  Shader shader;

  glm::vec3 cameraPos;
  glm::vec3 cameraFront;
  glm::vec3 cameraUp;
  glm::vec3 cameraRight;
  glm::mat4 view;

  glm::vec3 direction;

  float lastX;
  float lastY;

  float yaw;
  float pitch;

  bool firstMouse;

 public:
  Lucid(Registry* registry, Input* input, GLFWwindow* window);
  ~Lucid();

  void Update(double dt);

  bool IsKeyDown(int key);

  void HandleKeyCallback(GLFWwindow* window, int key, int scancode, int action,
                         int mods);
  void UpdateCameraVector(float xOffset, float yOffset);

 public:
  static void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Lucid* lucid = (Lucid*)glfwGetWindowUserPointer(window);

    if (lucid->firstMouse) {
      lucid->lastX = xpos;
      lucid->lastY = ypos;
      lucid->firstMouse = false;
    }

    float xOffset = xpos - lucid->lastX;
    float yOffset = lucid->lastY - ypos;

    lucid->lastX = xpos;
    lucid->lastY = ypos;

    lucid->UpdateCameraVector(xOffset, yOffset);
  }

  static void KeyCallback(GLFWwindow* window, int key, int scancode, int action,
                          int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    Lucid* lucid = (Lucid*)glfwGetWindowUserPointer(window);
    lucid->HandleKeyCallback(window, key, scancode, action, mods);
  }
};