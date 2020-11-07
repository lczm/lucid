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

  void InitializeGUI();
  void PresetLayout(ImGuiID dockSpaceID);
  void InitializeImGuiWindows();

  void DrawHierarchy();
  void DrawAssets();
  void DrawScene();
  void DrawProject();
  void DrawConsole();
  void DrawAnimation();
  void DrawAnimator();
  void DrawInspector();
  void DrawServices();
};