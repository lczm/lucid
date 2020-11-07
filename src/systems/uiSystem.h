#pragma once

#include <vector>
#include <iostream>

#include "gl.h"
#include "ecs.h"
#include "renderer.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class UiSystem : public System {
 private:
 public:
  UiSystem();
  ~UiSystem();

  void Update(double dt, Registry* registry, Input* input);

  void InitializeGUI(double dt, Registry* registry, Input* input);
  void PresetLayout(ImGuiID dockSpaceID);
  void InitializeImGuiWindows(double dt, Registry* registry, Input* input);

  void DrawHierarchy(double dt, Registry* registry, Input* input);
  void DrawAssets(double dt, Registry* registry, Input* input);
  void DrawScene(double dt, Registry* registry, Input* input);
  void DrawProject(double dt, Registry* registry, Input* input);
  void DrawConsole(double dt, Registry* registry, Input* input);
  void DrawAnimation(double dt, Registry* registry, Input* input);
  void DrawAnimator(double dt, Registry* registry, Input* input);
  void DrawInspector(double dt, Registry* registry, Input* input);
  void DrawServices(double dt, Registry* registry, Input* input);
};