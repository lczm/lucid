#pragma once

#include <limits>
#include <vector>
#include <iostream>

#include "shader.h"
#include "ecs.h"
#include "utils.h"
#include "renderer.h"
#include "shaderResource.h"
#include "cube.h"
#include "sphere.h"
#include "line.h"
#include "camera.h"
#include "primitiveVertex.h"
#include "engineComponents.h"
#include "renderUtils.h"
#include "colliders.h"

#include "gtx/string_cast.hpp"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class RenderSystem : public System
{
 private:
  Renderer* renderer;

// #if DEBUG
  Camera* camera;
// #endif

  uint32_t fbo, rbo;
  uint32_t texture;

  // This will be for the
  std::vector<glm::mat4> defaultBoneMatrices;

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
  void DrawAllColldiers(float dt, Registry* registry, Input* input);
  void DrawActiveEntityBoundingBox(float dt, Registry* registry, Input* input);

 public:
  // TODO : Typedef glm::vec3 to a ray as it feels overly verbose to declare
  // a vector as input/output
  std::tuple<bool, float> RayBoundingBoxCollisionCheck(glm::vec3 origin, glm::vec3 ray,
                                                       BoundingBox boundingBox);

 private:
  void InitSphereVerticesIndices(SphereVerticesIndices& sphereVerticesIndices);
};