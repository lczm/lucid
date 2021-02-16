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
#include "colliders.h"

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
static void InitUserEntities(Registry* registry, Input* input)
{
  registry->CreateResource<FpsRules>();
  FpsRules& fpsRules = registry->GetResource<FpsRules>();
  Entity soundId = registry->GetAvailableEntityId();
  Entity fontId = registry->GetAvailableEntityId();
  fpsRules.soundId = soundId;
  fpsRules.fontId = fontId;
  registry->CreateEntity<Sound>(soundId);
  registry->AddComponentData<Sound>(soundId, Sound(GRUNT_SOUND));
  registry->CreateEntity<Font, Transform>(fontId);
  registry->AddComponentData<Font>(fontId, Font(HELVETICA_FONT, "", 1));
  registry->GetComponent<Transform>(fpsRules.fontId).position = glm::vec3(10, 10, 0);

  input->lastX = input->GetMouseX();
  input->lastY = input->GetMouseY();

  for (size_t i = 0; i < 1; i++)
  {
    Entity enemyId = registry->GetAvailableEntityId();
    registry->CreateEntity<Model, Transform, RigidBody, Enemy>(enemyId);
    registry->AddComponentData<Model>(
        enemyId, Model("../examples/fps/assets/ugandan_knuckles/scene.gltf", registry));
    registry->GetComponent<RigidBody>(enemyId).applyGravity = false;
    registry->GetComponent<Transform>(enemyId).position.x += 50;
    registry->GetComponent<Transform>(enemyId).position.z = 8;
    registry->GetComponent<Transform>(enemyId).scale = glm::vec3(0.01);
    fpsRules.ugandanIds.push_back(enemyId);
  }
}