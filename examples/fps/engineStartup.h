#pragma once

#include "ecs.h"
#include "component.h"
#include "engineComponents.h"
#include "renderUtils.h"

#include "renderSystem.h"
#include "audioSystem.h"
#include "physicsSystem.h"
#include "fpsSystem.h"

// Initialize all your systems here
// This gets ran at startup
static void InitSystems(Registry* registry)
{
  DisableCursor(registry);
  registry->RegisterSystem(new FpsSystem(), "fps", 1);
  registry->RegisterSystem(new PhysicsSystem(), "physics", 4);
  registry->RegisterSystem(new RenderSystem(registry), "render", 5);
  registry->RegisterSystem(new AudioSystem(), "audio", 6);
}

// Initialize all your entities here.
// This gets ran at startup.
static void InitUserEntities(Registry* registry)
{
}