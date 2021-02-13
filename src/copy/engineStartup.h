#pragma once

#include "ecs.h"
#include "component.h"
#include "engineComponents.h"

#include "renderSystem.h"
#include "audioSystem.h"
#include "uiSystem.h"
#include "physicsSystem.h"

static void InitSystems(Registry* registry)
{
#if DEBUG
  registry->RegisterSystem(new UiSystem(), "ui", 1);
#endif

  registry->RegisterSystem(new PhysicsSystem(), "physics", 4);
  registry->RegisterSystem(new RenderSystem(registry), "render", 5);
  registry->RegisterSystem(new AudioSystem(), "audio", 6);
}
