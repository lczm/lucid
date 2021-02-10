#pragma once
#include <GLFW/glfw3.h>
#include <iostream>

#include "constants.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

class Input
{
 public:
  GLFWwindow* window;
  double x;
  double y;
  bool enableInput;

  // All the glfw supported keys
  bool keys[350];
  // Left / Right / Middle
  bool mouseKeys[3];

  float lastX, lastY;
  int scroll;

  WindowType activeWindow;

 public:
  Input();
  Input(GLFWwindow* window);
  ~Input();

 public:
  float GetMouseX();
  float GetMouseY();
  float GetMouseYAbsolute();
  int GetScrollState();

 public:
  bool IsKeyDown(int key);
  void SetKeyOn(int key);
  void SetKeyOff(int key);

  bool IsMouseLDown();
  bool IsMouseRDown();
  bool IsMouseMDown();
};
