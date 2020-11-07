#pragma once

#include <vector>
#include <iostream>

#include "gl.h"
#include "ecs.h"
#include "renderer.h"
#include "camera.h"

class RenderSystem : public System {
 private:
  Renderer* renderer;
  Camera* camera;

  uint32_t fbo, rbo;
  uint32_t texture;

 public:
  RenderSystem();
  ~RenderSystem();

  void Update(double dt, Registry* registry, Input* input);

  void HandleMousePan(double dt, Input* input);
  void HandleKeyboardPan(double dt, Input* input);
};