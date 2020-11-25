#pragma once

#include <vector>
#include <iostream>

#include "gl.h"
#include "ecs.h"
#include "renderer.h"
#include "camera.h"
#include "shaderResource.h"
#include "cube.h"
#include "sphere.h"
#include "devStructs.h"
#include "quatCamera.h"

#include "gtx/string_cast.hpp"

class RenderSystem : public System {
 private:
  Renderer* renderer;
  Camera* camera;
  QuatCamera* quatCamera;

  uint32_t fbo, rbo;
  uint32_t texture;

 public:
  RenderSystem();
  ~RenderSystem();

  void Update(double dt, Registry* registry, Input* input);

  void HandleMousePan(double dt, Input* input);
  void HandleMousePicking(double dt, Input* input);
  void HandleKeyboardPan(double dt, Input* input);
  void HandleMouseScroll(double dt, Input* input);

  void HandleKeyboardInput(double dt, Registry* registry, Input* input);

  void DrawAllLines(double dt, Registry* registry, Input* input);
  void DrawAllModels(double dt, Registry* registry, Input* input);
  void DrawAllCubes(double dt, Registry* registry, Input* input);
  void DrawAllSpheres(double dt, Registry* registry, Input* input);

  // Debugging purposes for now?
  void DrawAllBoundingBoxes(double dt, Registry* registry, Input* input);
};