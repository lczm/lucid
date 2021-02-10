#pragma once

#include "shapes.h"
#include "glm.hpp"
#include "commonEnum.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuizmo.h"
#include "uiUtils.h"

/*
 * This is mainly structs used to pass data around in the various systems.
 * These structs should not be included in the release builds but rather only for the debug builds
 * e.g. when the renderSystem needs to know if a certain checkbox is clicked from the uisystem, this
 * is where the structs are stored.
 */

struct DevDebug
{
  bool drawWireframe = false;
  bool drawColliders = false;

  // fps is updated per frame
  uint32_t fps = 0;
  uint32_t rayID = 0;
  uint32_t activeEntity = NONE_ACTIVE_ENTITY;

  bool onGizmo = false;
  WindowType changeFocusWindow = WindowType::None;
  ImGuizmo::OPERATION gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
};

// These values are updated on a per frame basis
struct RendererStats
{
  uint32_t calls;
  uint32_t vertices;
  uint32_t indices;
};

struct WidgetLayout
{
  float sceneWidth = 0;
  float sceneHeight = 0;

  float leftWindowWidth = 0;
  float leftWindowHeight = 0;

  float rightWindowWidth = 0;
  float rightWindowHeight = 0;

  float bottomWindowWidth = 0;
  float bottomWindowHeight = 0;

  float topWindowWidth = 0;
  float topWindowHeight = 0;

  float menuBarWidth = 0;
  float menuBarHeight = 0;
};

struct PrimitiveBatchIds
{
  // Lines
  uint32_t lineVAO, lineVBO = 0;

  // Spheres
  uint32_t sphereVAO, sphereVBO, sphereEBO = 0;

  // Cubes
  uint32_t cubeVAO, cubeVBO, cubeEBO = 0;
};

struct GridLine
{
  Line line;
};

// This is a special struct that will be initialized with the
// default positions and vertices of a sphere primtiive when
// initialized on run-time. The reason for this is because
// the vertices and indices of a sphere is relatively special
// compared to a cube or a line.
struct SphereVerticesIndices
{
  const float radius = 1.0f;
  const int sectors = 36;
  const int stacks = 18;

  std::vector<float> vertices;
  std::vector<uint32_t> indices;
};

// This GameState specifically refers to the in-game 'scene' state
enum class GameState
{
  PLAYING,  // The systems are being updated
  PAUSED,   // The systems are not being updated
};

struct GameEngineState
{
  GameState gameState = GameState::PAUSED;
};

struct RigidBodyConfiguration
{
  float gravityRate = 0.10f;
};

struct Workspace
{
  Node* absoluteProjectRoot = NewNode("", true);
  Node* relativeProjectRoot = NewNode("", true);
};

struct Listener
{
  float x = 0;
  float y = 0;
  float z = 0;
  float gain = 1;
};