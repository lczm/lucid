#pragma once

#include <chrono>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <typeinfo>
#include <vector>

#include "gl.h"
#include "ecs.h"
#include "input.h"
#include "constants.h"
#include "utils.h"

#if DEBUG
#include "engineComponents.h"
#endif

#include "model.h"
#include "line.h"
#include "cube.h"
#include "sphere.h"
#include "shaderResource.h"

// Demo imports
#include "pong.h"
#include "aiSystem.h"
#include "playerSystem.h"
#include "pongSystem.h"
// Demo imports end

#include "glm.hpp"
#include "gtx/string_cast.hpp"
#include "gtc/matrix_transform.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "renderSystem.h"
#include "lucidSystem.h"
#include "audioSystem.h"
#include "uiSystem.h"
#include "physicsSystem.h"

class Lucid
{
 public:
  Registry* registry;
  Input* input;
  GLFWwindow* window;

  std::chrono::high_resolution_clock::time_point timer;
  float dt, secondDt = 0;
  int frameCount = 0;

  std::function<void(GLFWwindow* window, int button, int action, int mods)> mouseCallback;
  std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)> keyCallback;
  std::function<void(GLFWwindow* window, double xoffset, double yoffset)> scrollCallback;

 public:
  Lucid(Registry* registry, Input* input, GLFWwindow* window);
  ~Lucid();

  void Update();

  void InitializeArchetypes();
  void InitializeBulitInEntities();
  void InitializeBuiltInSystems();

  void InitializeModelEntities();
  void InitializeSystems();

  // Demo pong game
  void InitializeDemoPongEntities();
  void InitializeDemoPongSystems();

#if DEBUG
  void InitializeSceneGridLines();
#endif

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