#include "input.h"

Input::Input(GLFWwindow* window) {
  Input::window = window;

  for (bool& key : keys) {
    key = false;
  }

  for (bool& mouseKey : mouseKeys) {
    mouseKey = false;
  }
}

Input::~Input() {
}

double Input::GetMouseX() {
  glfwGetCursorPos(window, &x, &y);
  return x;
}

double Input::GetMouseY() {
  glfwGetCursorPos(window, &x, &y);
  // OpenGL uses inverse y values compared to glfw window values
  return std::abs(SCREEN_HEIGHT - y);
}

bool Input::IsKeyDown(int key) {
  // This sets everything to capitalised ascii numbers
  // lets us use something like
  // input->isKeyDown('l')
  // input->isKeyDown('L')
  // I guess in the future if we need to do mod keys, this will have to be
  // changed
  if (key >= 97) {
    key -= 32;
  }
  return keys[key];
}

void Input::SetKeyOn(int key) {
  if (key >= 97) {
    key -= 32;
  }
  keys[key] = true;
}

void Input::SetKeyOff(int key) {
  if (key >= 97) {
    key -= 32;
  }
  keys[key] = false;
}

bool Input::IsMouseLDown() {
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
    return true;
  }
  return false;
}

bool Input::IsMouseRDown() {
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
    return true;
  }
  return false;
}

bool Input::IsMouseMDown() {
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)) {
    return true;
  }
  return false;
}
