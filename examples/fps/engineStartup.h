#pragma once

#include "ecs.h"
#include "component.h"
#include "engineComponents.h"
#include "renderUtils.h"

#include "renderSystem.h"
#include "audioSystem.h"
#include "physicsSystem.h"
#include "fpsSystem.h"
#include "rules.h"

// Initialize all your systems here
// This gets ran at startup
static void InitSystems(Registry* registry)
{
  DisableCursor(registry);
  SetMousePosition(registry, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

  registry->RegisterSystem(new FpsSystem(), "fps", 1);
  registry->RegisterSystem(new PhysicsSystem(), "physics", 4);
  registry->RegisterSystem(new RenderSystem(registry), "render", 5);
  registry->RegisterSystem(new AudioSystem(), "audio", 6);
}

// Initialize all your entities here.
// This gets ran at startup.
static void InitUserEntities(Registry* registry)
{
  registry->CreateResource<FpsRules>();
  FpsRules& fpsRules = registry->GetResource<FpsRules>();
  Entity soundId = registry->GetAvailableEntityId();
  Entity fontId = registry->GetAvailableEntityId();
  fpsRules.soundId = soundId;
  fpsRules.fontId = fontId;
  registry->CreateEntity<Sound>(soundId);
  registry->AddComponentData<Sound>(soundId, Sound(GRUNT_SOUND));
}