#pragma once

#include <limits>
#include <vector>
#include <iostream>

#include "shader.h"
#include "ecs.h"
#include "utils.h"
#include "renderer.h"
#include "camera.h"
#include "shaderResource.h"
#include "cube.h"
#include "sphere.h"
#include "line.h"
#include "quatCamera.h"
#include "renderUtils.h"
#include "primitiveVertex.h"
#include "engineComponents.h"

#if DEBUG
#include "dbg.h"
#endif

#include "gtx/string_cast.hpp"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class RenderSystem : public System
{
 private:
  Renderer* renderer;

#if DEBUG
  QuatCamera* quatCamera;
#endif

  uint32_t fbo, rbo;
  uint32_t texture;

 public:
  RenderSystem(Registry* registry);
  ~RenderSystem();

  void Update(float dt, Registry* registry, Input* input);

  void InitRenderBuffers();
  void InitSceneCameraComponent(Registry* registry);
  void InitPrimitiveBuffers(Registry* registry);

  void HandleMousePan(float dt, Registry* registry, Input* input);
  void HandleKeyboardPan(float dt, Input* input);
  void HandleMouseScroll(float dt, Input* input);

  bool HandleMousePick(float dt, Registry* registry, Input* input);
  void DrawAllLines(float dt, Registry* registry, Input* input);
  void DrawAllModels(float dt, Registry* registry, Input* input);
  void DrawAllCubes(float dt, Registry* registry, Input* input);
  void DrawAllSpheres(float dt, Registry* registry, Input* input);

  // This draws a bounding box around everything that has a transform
  // Main used for debugging purposes though.
  void DrawAllBoundingBoxes(float dt, Registry* registry, Input* input);
  void DrawAllColldiers(float dt, Registry* registry, Input* input);

 public:
  // TODO : Typedef glm::vec3 to a ray as it feels overly verbose to declare
  // a vector as input/output
  glm::vec3 GetRayDirection(Registry* registry, Input* input);
  std::tuple<bool, float> RayBoundingBoxCollisionCheck(glm::vec3 origin, glm::vec3 ray,
                                                       BoundingBox boundingBox);

  BoundingBox GetBoundingBox(std::vector<glm::vec4> vertices);

 private:
  void InitSphereVerticesIndices(SphereVerticesIndices& sphereVerticesIndices);
};