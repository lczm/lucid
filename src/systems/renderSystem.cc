#include "renderSystem.h"

RenderSystem::RenderSystem() {
  RenderSystem::renderer = new Renderer();
}

RenderSystem::~RenderSystem() = default;

void RenderSystem::Update(double dt, Registry* registry, Input* input) {
}