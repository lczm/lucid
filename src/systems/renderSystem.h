#pragma once

#include <vector>
#include <iostream>

#include "gl.h"
#include "ecs.h"
#include "renderer.h"

class RenderSystem : public System {
 private:
  Renderer* renderer;
  Shader modelShader;

 public:
  RenderSystem();
  ~RenderSystem();

  void Update(double dt, Registry* registry, Input* input);
};