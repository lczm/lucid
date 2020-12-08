#pragma once

#include "windowEnum.h"
#include "glm.hpp"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuizmo.h"

/*
 * This is mainly structs used to pass data around in the various systems.
 * These structs should not be included in the release builds but rather only for the debug builds
 * e.g. when the renderSystem needs to know if a certain checkbox is clicked from the uisystem, this
 * is where the structs are stored.
 */

struct DevDebug {
  bool drawWireframe = false;
  bool drawColliders = false;

  bool onGizmo = false;

  // fps is updated per frame
  uint32_t fps = 0;
  uint32_t rayID = 0;

  uint32_t activeEntity = 0;

  float sceneWidth = 0;
  float sceneHeight = 0;

  float leftWindowWidth = 0;
  float leftWindowHeight = 0;

  float rightWindowWidth = 0;
  float rightWindowHeight = 0;

  float bottomWindowWidth = 0;
  float bottomWindowHeight = 0;

  float menuBarWidth = 0;
  float menuBarHeight = 19;

  WindowType changeFocusWindow = WindowType::None;
  ImGuizmo::OPERATION gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;

  // Temporary, Need to move the camera into an entity
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);
};
