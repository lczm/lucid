#include "input.h"

Input::Input(GLFWwindow* window) {
  Input::window = window;

  // glfwSetWindowUserPointer(window, this);
  // glfwSetKeyCallback(window, KeyCallback);
  // glfwSetCursorPosCallback(window, MouseCallback);

  for (size_t i = 0; i < 350; i++) {
    keys[i] = false;
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

void Input::HandleKeyCallback(GLFWwindow* window, int key, int scancode,
                              int action, int mods) {
  // Dont handle unknown keys
  if (key == GLFW_KEY_UNKNOWN) return;

  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    keys[key] = true;
  } else if (action == GLFW_RELEASE) {
    keys[key] = false;
  }
}

// void Input::HandleMouseCallback(GLFWwindow* window, double xpos, double ypos) {
//   if (firstMouse) {
//     lastX = xpos;
//     lastY = ypos;
//     firstMouse = false;
//   }
//
//   float xOffset = xpos - lastX;
//   float yOffset = ypos - lastY;
//
//   lastX = xpos;
//   lastY = ypos;
//
//   const float sensitivity = 0.01f;
//   xOffset *= sensitivity;
//   yOffset *= sensitivity;
//
//   yaw += xOffset;
//   pitch += yOffset;
//
//   if (pitch > 89.0f) pitch = 89.0f;
//   if (pitch < -89.0f) pitch = -89.0f;
//
//   direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//   direction.y = sin(glm::radians(pitch));
//   direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
//   // cameraFront = glm::normalize(direction);
// }