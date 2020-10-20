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

class Sandbox {
 private:
  Registry* registry;
  Input* input;
  GLFWwindow* window;

  uint32_t VAO, VBO, EBO;
  uint32_t vertexShader, fragmentShader;
  uint32_t shaderProgram;

  Shader shader;

  glm::vec3 cameraPos;
  glm::vec3 cameraFront;
  glm::vec3 cameraUp;
  glm::mat4 view;

 public:
  Sandbox(Registry* registry, Input* input, GLFWwindow* window);
  ~Sandbox();

  void Update(double dt);
};