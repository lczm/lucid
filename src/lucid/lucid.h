#pragma once

#include <functional>

#include "gl.h"
#include "ecs.h"
#include "input.h"
#include "model.h"
#include "constants.h"
#include "glm.hpp"
#include "gtx/string_cast.hpp"
#include "gtc/matrix_transform.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "renderSystem.h"
#include "lucidSystem.h"

class Lucid {
 public:
  Registry* registry;
  Input* input;
  GLFWwindow* window;

  // There are ~350 keys defined by GLFW
  bool keys[350];

  // Left / Right / Middle
  bool mouseKeys[3];

  uint32_t VAO, VBO, EBO;
  uint32_t vertexShader, fragmentShader;

  Shader shader;

  glm::vec3 cameraPos;
  glm::vec3 cameraFront;
  glm::vec3 cameraUp;
  glm::vec3 cameraRight;
  glm::mat4 view;

  uint32_t modelShaderID;

  float lastX;
  float lastY;

  float yaw;
  float pitch;

  std::function<void(GLFWwindow* window, int button, int action, int mods)> mouseCallback;
  std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)> keyCallback;

 public:
  Lucid(Registry* registry, Input* input, GLFWwindow* window);
  ~Lucid();

  void Update(double dt);

  bool IsKeyDown(int key);

  void UpdateCameraVector(float xOffset, float yOffset);

  static void HandleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void HandleMouseCallback(GLFWwindow* window, int button, int action, int mods);

  void SetMouseCallback(
      std::function<void(GLFWwindow* window, int button, int action, int mods)> fn);
  void SetKeyCallback(
      std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)> fn);

  static void MouseCallback(GLFWwindow* window, int button, int action, int mods);
  static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};