#pragma once

#include <functional>

#include "gl.h"
#include "ecs.h"
#include "input.h"
#include "constants.h"

// TODO : use compiler if debug flag here
#include "devStructs.h"

#include "model.h"
#include "cube.h"
#include "sphere.h"
#include "shaderResource.h"

#include "glm.hpp"
#include "gtx/string_cast.hpp"
#include "gtc/matrix_transform.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "renderSystem.h"
#include "lucidSystem.h"
#include "uiSystem.h"

class Lucid {
 public:
  Registry* registry;
  Input* input;
  GLFWwindow* window;

  std::function<void(GLFWwindow* window, int button, int action, int mods)> mouseCallback;
  std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)> keyCallback;
  std::function<void(GLFWwindow* window, double xoffset, double yoffset)> scrollCallback;

 public:
  Lucid(Registry* registry, Input* input, GLFWwindow* window);
  ~Lucid();

  void Update(double dt);
  void InitializeEntities();
  void InitializeSystems();

  void SetMouseCallback(
      std::function<void(GLFWwindow* window, int button, int action, int mods)> fn);
  void SetKeyCallback(
      std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)> fn);
  void SetScrollCallback(
      std::function<void(GLFWwindow* window, double xOffset, double yOffset)> fn);

  static void HandleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void HandleMouseCallback(GLFWwindow* window, int button, int action, int mods);
  static void HandleScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

  static void MouseCallback(GLFWwindow* window, int button, int action, int mods);
  static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
};