#pragma once

#include "ecs.h"
#include "component.h"
#include "engineComponents.h"

#include "cube.h"
#include "model.h"
#include "shader.h"
#include "sphere.h"
#include "quatCamera.h"
#include "shaderResource.h"

#if DEBUG
#include "pong.h"
#endif

// TODO : This will not be needed anymore, when in the future
// RegisterArchetype will be built-in and implicit.
static void InitArchetypes(Registry* registry)
{
  // Note that the archetypes registered under here and that are not
  // under the #if DEBUG flag are the ones that will also exist
  // in the release build of the game.
  registry->RegisterArchetype<ShaderResource>();
  registry->RegisterArchetype<PrimitiveBatchIds>();
  registry->RegisterArchetype<Model, Transform>();
  registry->RegisterArchetype<QuatCamera>();
  registry->RegisterArchetype<SphereVerticesIndices>();
  registry->RegisterArchetype<Line, Transform>();
  registry->RegisterArchetype<GameEngineState>();
  registry->RegisterArchetype<RendererStats>();

  // Register the necessary archetypes
  // Note that this should be using `ColliderCube` but it is hardcoded like this for now.
  registry->RegisterArchetype<Cube, Transform, RigidBody>();
  registry->RegisterArchetype<Cube, Transform, RigidBody, ColliderCube>();
  registry->RegisterArchetype<Cube, Transform, RigidBody, ColliderSphere>();
  registry->RegisterArchetype<Sphere, Transform, RigidBody, ColliderCube>();
  registry->RegisterArchetype<PongRules>();

  // Default archetypes for gui interface
  registry->RegisterArchetype<Sphere, Transform>();
  registry->RegisterArchetype<Cube, Transform>();

  registry->RegisterArchetype<SoundEffect, Transform>();
  registry->RegisterArchetype<Music>();

  registry->RegisterArchetype<RigidBodyConfiguration>();

#if DEBUG
  registry->RegisterArchetype<SceneRender>();
  registry->RegisterArchetype<DevDebug>();
  registry->RegisterArchetype<WidgetLayout>();
  registry->RegisterArchetype<GridLine, Transform>();
#endif
}

static void InitEngineComponents(Registry* registry)
{
  Entity primitiveBatchID = registry->GetAvailableEntityId();
  Entity shaderResourceID = registry->GetAvailableEntityId();

  registry->CreateEntity<PrimitiveBatchIds>(primitiveBatchID);
  registry->CreateEntity<ShaderResource>(shaderResourceID);

  ShaderResource& shaderResource = registry->GetComponent<ShaderResource>();
  shaderResource.modelShader.CreateShader(MODEL_VERTEX_SHADER, MODEL_FRAGMENT_SHADER);
  shaderResource.triangleShader.CreateShader(TRIANGLE_VERTEX_SHADER, TRIANGLE_FRAGMENT_SHADER);
  shaderResource.primitiveShader.CreateShader(PRIMITIVE_VERTEX_SHADER, PRIMITIVE_FRAGMENT_SHADER);
  shaderResource.primitiveShaderBatch.CreateShader(PRIMITIVE_LINE_SHADER,
                                                   PRIMITIVE_FRAGMENT_SHADER);
  shaderResource.cubeShaderBatch.CreateShader(PRIMITIVE_CUBE_SHADER, PRIMITIVE_FRAGMENT_SHADER);
  // Note : using the primitive cube shader for now? If there turns out to not be a need
  // for the sphere and cubes to use different shaders, then simplify the shader names.
  shaderResource.sphereShaderBatch.CreateShader(PRIMITIVE_CUBE_SHADER, PRIMITIVE_FRAGMENT_SHADER);

  Entity sphereVerticesIndicesID = registry->GetAvailableEntityId();
  registry->CreateEntity<SphereVerticesIndices>(sphereVerticesIndicesID);

  Entity gameEngineStateID = registry->GetAvailableEntityId();
  registry->CreateEntity<GameEngineState>(gameEngineStateID);

  Entity rigidBodyConfigurationID = registry->GetAvailableEntityId();
  registry->CreateEntity<RigidBodyConfiguration>(rigidBodyConfigurationID);

#if DEBUG
  Entity sceneRenderID = registry->GetAvailableEntityId();
  Entity devDebugID = registry->GetAvailableEntityId();
  Entity widgetLayoutID = registry->GetAvailableEntityId();
  Entity rendererStatsID = registry->GetAvailableEntityId();

  registry->CreateEntity<SceneRender>(sceneRenderID);
  registry->CreateEntity<DevDebug>(devDebugID);
  registry->CreateEntity<WidgetLayout>(widgetLayoutID);
  registry->CreateEntity<RendererStats>(rendererStatsID);
#endif
}
