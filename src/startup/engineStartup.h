#pragma once

#include "ecs.h"
#include "component.h"
#include "engineComponents.h"

#include "model.h"
#include "shader.h"
#include "shapes.h"
#include "colliders.h"
#include "camera.h"

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

  std::cout << "initsystems from lucid" << std::endl;

  registry->RegisterSystem(new PhysicsSystem(), "physics", 4);

  // Demo start -- PongSystem will need to deal with collision for the ball
  // registry->RegisterSystem(new PongSystem(), "pong", 5);
  // Demo end

  registry->RegisterSystem(new RenderSystem(registry), "render", 5);
  registry->RegisterSystem(new AudioSystem(), "audio", 6);
}

// TODO : This will not be needed anymore, when in the future
// RegisterArchetype will be built-in and implicit.
static void InitArchetypes(Registry* registry)
{
  // Note that the archetypes registered under here and that are not
  // under the #if DEBUG flag are the ones that will also exist
  // in the release build of the game.
  registry->RegisterEmptyArchetype();
  // registry->RegisterArchetype<ShaderResource>();
  // registry->RegisterArchetype<PrimitiveBatchIds>();
  // registry->RegisterArchetype<Model, Transform>();
  // registry->RegisterArchetype<Model, Transform, ColliderCube>();
  // registry->RegisterArchetype<Model, Transform, ColliderPolygon>();
  // registry->RegisterArchetype<Camera>();
  // registry->RegisterArchetype<SphereVerticesIndices>();
  // registry->RegisterArchetype<Line, Transform>();
  // registry->RegisterArchetype<GameEngineState>();
  // registry->RegisterArchetype<RendererStats>();

  // Register the necessary archetypes
  // Note that this should be using `ColliderCube` but it is hardcoded like this for now.
  // registry->RegisterArchetype<Cube, Transform, RigidBody>();
  // registry->RegisterArchetype<Cube, Transform, RigidBody, ColliderCube>();
  // registry->RegisterArchetype<Cube, Transform, RigidBody, ColliderSphere>();
  // registry->RegisterArchetype<Sphere, Transform, RigidBody, ColliderCube>();
  // registry->RegisterArchetype<Sphere, Transform, RigidBody, ColliderSphere>();
  // registry->RegisterArchetype<PongRules>();

  // Default archetypes for gui interface
  // registry->RegisterArchetype<Sphere, Transform>();
  // registry->RegisterArchetype<Cube, Transform>();

  // registry->RegisterArchetype<Sound, Transform>();
  // registry->RegisterArchetype<RigidBodyConfiguration>();

#if DEBUG
  registry->RegisterArchetype<SceneRender>();
  registry->RegisterArchetype<DevDebug>();
  registry->RegisterArchetype<WidgetLayout>();
  registry->RegisterArchetype<GridLine, Transform>();
#endif
}

static void InitEngineComponents(Registry* registry)
{
  // Editor resources are for the engine devs to use
  registry->CreateEditorResource<PrimitiveBatchIds>();
  registry->CreateEditorResource<SphereVerticesIndices>();
  registry->CreateEditorResource<Workspace>();
  registry->CreateEditorResource<Listener>();
  registry->CreateEditorResource<SceneRender>();
  registry->CreateEditorResource<DevDebug>();
  registry->CreateEditorResource<WidgetLayout>();
  registry->CreateEditorResource<RendererStats>();
  registry->CreateEditorResource<GameEngineState>();
  // Scene Camera
  registry->CreateEditorResource<Camera>();
  // In Game Camera Model
  registry->CreateEditorResource<Model>();

  // Resources are for the users to use
  registry->CreateResource<RigidBodyConfiguration>();
  registry->CreateResource<ShaderResource>();
  // Game Camera
  registry->CreateResource<Camera>();

  // Setup the In-Game camera
  registry->GetEditorResource<Model>().registry = registry;
  registry->GetEditorResource<Model>().path = CAMERA_MODEL;
  registry->GetEditorResource<Model>().Load();
  registry->GetResource<Camera>().transform.position += glm::vec3(0.0f, 1.0f, 5.0f);
  registry->GetResource<Camera>().transform.scale /= 80.0f;

  ShaderResource& shaderResource = registry->GetResource<ShaderResource>();

  shaderResource.modelShader.CreateShader(MODEL_VERTEX_SHADER, MODEL_FRAGMENT_SHADER);
  shaderResource.modelAnimatedShader.CreateShader(MODEL_ANIMATED_VERTEX_SHADER,
                                                  MODEL_FRAGMENT_SHADER);

  shaderResource.primitiveShader.CreateShader(PRIMITIVE_VERTEX_SHADER, PRIMITIVE_FRAGMENT_SHADER);
  shaderResource.lineShader.CreateShader(PRIMITIVE_LINE_SHADER, PRIMITIVE_FRAGMENT_SHADER);

  shaderResource.cubeShader.CreateShader(PRIMITIVE_INSTANCED_SHADER, PRIMITIVE_FRAGMENT_SHADER);
  // Note : using the primitive cube shader for now? If there turns out to not be a need
  // for the sphere and cubes to use different shaders, then simplify the shader names.
  shaderResource.sphereShader.CreateShader(PRIMITIVE_INSTANCED_SHADER, PRIMITIVE_FRAGMENT_SHADER);
}
