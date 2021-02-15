#pragma once

#include <vector>
#include <iostream>

#include "constants.h"
#include "shader.h"
#include "ecs.h"
#include "renderer.h"
#include "renderUtils.h"
#include "shapes.h"
#include "colliders.h"

#include "camera.h"
#include "startup.h"
#include "engineComponents.h"

#include "glm.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/quaternion.hpp"
#include "gtx/matrix_decompose.hpp"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuizmo.h"
#include "nfd.h"
#include "uiUtils.h"
#include "serialize.h"
#include "systemUtils.h"
#include "font.h"
#include "sound.h"

namespace fs = std::filesystem;

class UiSystem : public System
{
 private:
  bool drawWireframe = false;
  bool drawSceneOnly = false;
  bool drawSelectFolderPopup = false;
  Node* absoluteProjectRoot = NewNode("", true);
  fs::path lucidBuildPath = fs::current_path().parent_path();
  const std::vector<std::pair<DefaultAssetsType, std::string>> defaultAssets = {
      {DefaultAssetsType::Cube, "Cube"}, {DefaultAssetsType::Sphere, "Sphere"}};
  std::vector<std::string> addComponentItems = {"Model",          "Transform",      "RigidBody",
                                                "Sound",          "Font",           "ColliderCube",
                                                "ColliderSphere", "ColliderPolygon"};

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
  void DrawInspector(float dt, Registry* registry, Input* input);
  void DrawDevDebug(float dt, Registry* registry, Input* input);
  void DrawToolBar(float dt, Registry* registry, Input* input);
  void DrawDefaultAssets(float dt, Registry* registry, Input* input);
  void DrawGameConfiguration(float dt, Registry* registry, Input* input);
  void DrawProgressBar(float fraction, std::string message);
  void DrawInspectorCubeComponent(
      Registry* registry, DevDebug& devDebug,
      ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen);
  void DrawInspectorSphereComponent(
      Registry* registry, DevDebug& devDebug,
      ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen);
  void DrawInspectorTransformComponent(
      Registry* registry, DevDebug& devDebug,
      ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen);
  void DrawInspectorRigidBodyComponent(
      Registry* registry, DevDebug& devDebug, Input* input,
      ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen);
  void DrawInspectorSoundComponent(
      Registry* registry, DevDebug& devDebug, Input* input,
      ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen);
  void DrawInspectorModelComponent(
      Registry* registry, DevDebug& devDebug, Input* input,
      ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen);
  void DrawInspectorColliderCubeComponent(
      Registry* registry, DevDebug& devDebug, Input* input,
      ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen);
  void DrawInspectorColliderSphereComponent(
      Registry* registry, DevDebug& devDebug, Input* input,
      ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen);
  void DrawInspectorColliderPolygonComponent(
      Registry* registry, DevDebug& devDebug, Input* input,
      ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen);
  void DrawInspectorAddComponent(Registry* registry,
                                 ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen);

  void DisableInputWhenDragScrollbar(Registry* registry, Input* input);
  void HandleGizmoInput(Registry* registry, Input* input);
  void UpdateSceneWindow(Registry* registry, Input* input);
  void UpdateGizmoType(Registry* registry, Input* input);
  void UpdateInputActiveWindow(Registry* registry, Input* input, WindowType windowType);
  void UpdateWindowFocus(Registry* registry, WindowType windowType, std::string focusWindow,
                         Input* input, WindowType changeFocusWindow = WindowType::None);
  void OpenProject(Registry* registry, Input* input);
};