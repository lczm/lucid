#pragma once

#include "ecs.h"
#include "component.h"
#include "engineComponents.h"

// #if DEBUG
// #include "pong.h"
// #include "aiSystem.h"
// #include "playerSystem.h"
// #include "pongSystem.h"
// #endif

#include "renderSystem.h"
#include "audioSystem.h"
#include "uiSystem.h"
#include "physicsSystem.h"

static void InitSystems(Registry* registry)
{
  // TODO : Figure this out
  // UiSystem can either be before or after rendersystem.
  // It does somewhat make sense that the uisystem will be after the render system
  // going by the logic of :
  // renderSystem generates a frameBuffer object that the uiSystem can immediately use after
  // But I also recall that I had a specific reason for setting UiSystem to be first
  // perhaps double buffering...?

#if DEBUG
  registry->RegisterSystem(new UiSystem(), "ui", 1);
#endif

  // Demo start -- TODO : This should be separated, need a way of prioritising systems
  // registry->RegisterSystem(new PlayerSystem(), "player", 2);
  // registry->RegisterSystem(new AiSystem(), "ai", 3);
  // Demo end

  registry->RegisterSystem(new PhysicsSystem(), "physics", 4);

  // Demo start -- PongSystem will need to deal with collision for the ball
  // registry->RegisterSystem(new PongSystem(), "pong", 5);
  // Demo end

  registry->RegisterSystem(new RenderSystem(registry), "render", 5);
  registry->RegisterSystem(new AudioSystem(), "audio", 6);
}

static void InitUserEntities(Registry* registry, Input* input)
{
}