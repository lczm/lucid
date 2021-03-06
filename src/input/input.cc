#include "input.h"

// Mostly used for mock / tests
Input::Input()
{
}

Input::Input(GLFWwindow* window)
{
  Input::window = window;
  enableInput = true;

  for (bool& key : keys)
  {
    key = false;
  }

  for (bool& mouseKey : mouseKeys)
  {
    mouseKey = false;
  }

  // Note to change this back to none once done with debugging
  activeWindow = WindowType::Scene;
}

Input::~Input()
{
}

float Input::GetMouseX()
{
  glfwGetCursorPos(window, &x, &y);

#if DEBUG
  if ((activeWindow == WindowType::Scene || activeWindow == WindowType::GameCamera) && enableInput)
  {
    // When it is defined that it is in 'DEBUG' mode,
    // the coordinates needs to be offseted
    return static_cast<float>(x);
  }
#endif

#if RELEASE
  return static_cast<float>(x);
#endif

  return static_cast<float>(x);
}

float Input::GetMouseY()
{
  glfwGetCursorPos(window, &x, &y);

#if DEBUG
  if ((activeWindow == WindowType::Scene || activeWindow == WindowType::GameCamera) && enableInput)
  {
    return std::abs(SCREEN_HEIGHT - static_cast<float>(y));
    // return y;
  }
#endif

#if RELEASE
  return std::abs(SCREEN_HEIGHT - y);
#endif

  // OpenGL uses inverse y values compared to glfw window values
  return std::abs(SCREEN_HEIGHT - static_cast<float>(y));
}

float Input::GetMouseYAbsolute()
{
  glfwGetCursorPos(window, &x, &y);

#if DEBUG
  if ((activeWindow == WindowType::Scene || activeWindow == WindowType::GameCamera) && enableInput)
  {
    return static_cast<float>(y);
  }
#endif

#if RELEASE
  return y;
#endif

  // OpenGL uses inverse y values compared to glfw window values
  return static_cast<float>(y);
}

int Input::GetScrollState()
{
#if DEBUG
  if ((activeWindow == WindowType::Scene || activeWindow == WindowType::GameCamera) && enableInput)
  {
    return scroll;
  }
#endif

#if RELEASE
  return scroll;
#endif

  // Note : will never reach this, it is either DEBUG or RELEASE
  // return scroll;
  // Jasper: It will reach here in debug if the activewindow isnt scene or gamecamera
  return 0;
}

bool Input::IsKeyDown(int key)
{
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
  if ((activeWindow == WindowType::Scene || activeWindow == WindowType::GameCamera ||
       activeWindow == WindowType::Inspector) &&
      enableInput)
  {
    return keys[key];
  }
#endif

#if RELEASE
  return keys[key];
#endif

  return false;
}

void Input::SetKeyOn(int key)
{
  if (key >= 97)
  {
    key -= 32;
  }
  keys[key] = true;
}

void Input::SetKeyOff(int key)
{
  if (key >= 97)
  {
    key -= 32;
  }
  keys[key] = false;
}

bool Input::IsMouseLDown()
{
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
  {
#if DEBUG
    // TODO: fix debug and release being the same
    // Handle imgui windows
    if ((activeWindow == WindowType::Scene || activeWindow == WindowType::GameCamera) &&
        enableInput)
    {
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

bool Input::IsMouseRDown()
{
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
  {
#if DEBUG
    // Handle imgui windows
    if ((activeWindow == WindowType::Scene || activeWindow == WindowType::GameCamera) &&
        enableInput)
    {
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

bool Input::IsMouseMDown()
{
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE))
  {
    return true;
  }
  return false;
}
