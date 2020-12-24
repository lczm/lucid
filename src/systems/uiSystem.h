#pragma once

#include <vector>
#include <iostream>

#include "shader.h"
#include "ecs.h"
#include "utils.h"
#include "renderer.h"
#include "defaultAssetsEnum.h"
#include "windowEnum.h"
#include "renderUtils.h"
#include "cube.h"
#include "sphere.h"

#if DEBUG
#include "quatCamera.h"
#include "engineComponents.h"
#endif

#include "glm.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/quaternion.hpp"
#include "gtx/matrix_decompose.hpp"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuizmo.h"

class UiSystem : public System
{
 private:
  bool drawWireframe;
  bool drawSceneOnly;
  const std::vector<std::pair<DefaultAssetsType, std::string>> defaultAssets = {
      {DefaultAssetsType::Cube, "Cube"},
      {DefaultAssetsType::Sphere, "Sphere"},
      {DefaultAssetsType::Line, "Line"}};

 public:
  UiSystem();
  ~UiSystem();

  void Update(float dt, Registry* registry, Input* input);

  void InitializeGUI(float dt, Registry* registry, Input* input);
  void PresetLayout(ImGuiID dockSpaceID);
  void InitializeImGuiWindows(float dt, Registry* registry, Input* input);

  void DrawHierarchy(float dt, Registry* registry, Input* input);
  void DrawAssets(float dt, Registry* registry, Input* input);
  void DrawScene(float dt, Registry* registry, Input* input);
  void DrawGameCamera(float dt, Registry* registry, Input* input);
  void DrawProject(float dt, Registry* registry, Input* input);
  void DrawConsole(float dt, Registry* registry, Input* input);
  void DrawInspector(float dt, Registry* registry, Input* input);
  void DrawDevDebug(float dt, Registry* registry, Input* input);
  void DrawToolBar(float dt, Registry* registry, Input* input);
  void DrawDefaultAssets(float dt, Registry* registry, Input* input);
  void DrawGameConfiguration(float dt, Registry* registry, Input* input);
  void DrawProgressBar(float fraction, std::string message);

  void HandleGizmoInput(Registry* registry, Input* input);
  void UpdateSceneWindow(Registry* registry, Input* input);
  void UpdateGizmoType(Registry* registry, Input* input);
  void UpdateInputActiveWindow(Input* input, WindowType windowType);
  void UpdateWindowFocus(Registry* registry, WindowType windowType, const char* focusWindow,
                         Input* input, WindowType changeFocusWindow = WindowType::None);
};