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
  double getMouseX();
  double getMouseY();

 public:
  bool isKeyDown(int key);
  void setKeyOn(int key);
  void setKeyOff(int key);

  bool isMouseLDown();
  bool isMouseRDown();
  bool isMouseMDown();

  void handleKeyCallback(GLFWwindow* window, int key, int scancode, int action,
                         int mods);

 private:
  static void keyCallback(GLFWwindow* window, int key, int scancode, int action,
                          int mods) {
    // Quit the program
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    Input* input = (Input*)glfwGetWindowUserPointer(window);
    input->handleKeyCallback(window, key, scancode, action, mods);
  }
};
