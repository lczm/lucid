#include "input.h"

// Mostly used for mock / tests
Input::Input() {
}

Input::Input(GLFWwindow* window) {
  Input::window = window;

  for (bool& key : keys) {
    key = false;
  }

  for (bool& mouseKey : mouseKeys) {
    mouseKey = false;
  }

  activeWindow = WindowType::None;
}

Input::~Input() {
}

double Input::GetMouseX() {
  glfwGetCursorPos(window, &x, &y);

#if DEBUG
  if (activeWindow == WindowType::Scene) {
    // When it is defined that it is in 'DEBUG' mode,
    // the coordinates needs to be offseted
    return x;
  }
#endif

#if RELEASE
  return x;
#endif

  return x;
}

double Input::GetMouseY() {
  glfwGetCursorPos(window, &x, &y);

#if DEBUG
  if (activeWindow == WindowType::Scene) {
    return std::abs(SCREEN_HEIGHT - y);
    // return y;
  }
#endif

#if RELEASE
  return std::abs(SCREEN_HEIGHT - y);
#endif

  // OpenGL uses inverse y values compared to glfw window values
  return std::abs(SCREEN_HEIGHT - y);
}

int Input::GetScrollState() {
#if DEBUG
  if (activeWindow == WindowType::Scene) {
    return scroll;
  }
#endif

#if RELEASE
  return scroll;
#endif
}

bool Input::IsKeyDown(int key) {
  // This sets everything to capitalised ascii numbers
  // lets us use something like
  // input->isKeyDown('l')
  // input->isKeyDown('L')
  // I guess in the future if we need to do mod keys, this will have to be
  // changed

  // Don't really need this anymore...?
  // if (key >= 97) {
  //   key -= 32;
  // }

#if DEBUG
  if (activeWindow == WindowType::Scene) {
    return keys[key];
  }
#endif

#if RELEASE
  return keys[key];
#endif

  return false;
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
#if DEBUG
    // Handle imgui windows
    if (activeWindow == WindowType::Scene) {
      return true;
    }
#endif

#if RELEASE
    // Don't have to handle imgui windows
    return true;
#endif

    return false;
  }
  return false;
}

bool Input::IsMouseRDown() {
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
#if DEBUG
    // Handle imgui windows
    if (activeWindow == WindowType::Scene) {
      return true;
    }
#endif

#if RELEASE
    // Dont handle imgui windows
    return true;
#endif

    return false;
  }
  return false;
}

bool Input::IsMouseMDown() {
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)) {
    return true;
  }
  return false;
}
