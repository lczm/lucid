#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include "constants.h"

class Input {
 private:
  GLFWwindow* window;
  double x;
  double y;

  // All the glfw supported keys
  bool keys[350];

 public:
  Input(GLFWwindow* window);
  ~Input();

 public:
  double GetMouseX();
  double GetMouseY();

 public:
  bool IsKeyDown(int key);
  void SetKeyOn(int key);
  void SetKeyOff(int key);

  bool IsMouseLDown();
  bool IsMouseRDown();
  bool IsMouseMDown();

  void HandleKeyCallback(GLFWwindow* window, int key, int scancode, int action,
                         int mods);

 private:
  static void KeyCallback(GLFWwindow* window, int key, int scancode, int action,
                          int mods) {
    // Quit the program
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    Input* input = (Input*)glfwGetWindowUserPointer(window);
    input->HandleKeyCallback(window, key, scancode, action, mods);
  }
};
