#include "lucid.h"

struct Test
{
};

Lucid::Lucid(Registry* registry, Input* input, GLFWwindow* window)
{
  Lucid::registry = registry;
  Lucid::input = input;
  Lucid::window = window;

  timer = std::chrono::high_resolution_clock::now();

  // Target this window for user pointer for GLFW, this is so that
  // in callbacks, we can retrieve back the class
  glfwSetWindowUserPointer(window, this);

  // Set GLFW callback handlers
  glfwSetKeyCallback(window, KeyCallback);
  glfwSetMouseButtonCallback(window, MouseCallback);
  glfwSetScrollCallback(window, ScrollCallback);

  // Set Lucid callbacks
  SetMouseCallback(HandleMouseCallback);
  SetKeyCallback(HandleKeyCallback);
  SetScrollCallback(HandleScrollCallback);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;

  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(GLSL_VERSION);

  InitializeArchetypes();
  InitializeBuiltInEntities();
  InitializeBuiltInSystems();

  // InitializeModelEntities();
  // InitializeSystems();

  // Please remove this in the future, for batch rendering tests only
  // InitializeManyCubes();

#if DEBUG
  InitializeSceneGridLines();
#endif

  // TODO : This should be abstracted out into a user system
  // Demo pong
  InitializeDemoPongEntities();
  InitializeDemoPongSystems();
}

Lucid::~Lucid()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
};

void Lucid::Update()
{
  auto now = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> elapsed = now - timer;
  dt = elapsed.count();
  timer = now;

  // SecondDt here refers to 'seconds' in time measurements
  secondDt += dt;

  // TODO : Set max FPS
  // const float MAX_FPS = 60.0f;
  // const float MAX_PERIOD = 1.0 / MAX_FPS;

  // If it has been a second
  if (secondDt >= 1.0f)
  {
    // Update the internal fps counter before resetting it back to 0
    DevDebug& devDebug = registry->GetComponent<DevDebug>();
    devDebug.fps = frameCount;

    secondDt -= 1.0f;
    frameCount = 0;
  }

  // Get the current game state
  GameEngineState& gameEngineState = registry->GetComponent<GameEngineState>();

  // Only if the game state is 'playing', then update the systems.
  if (gameEngineState.gameState == GameState::PLAYING)
  {
    registry->UpdateSystems(dt, input);
  }
  else if (gameEngineState.gameState == GameState::PAUSED)
  {
    // If it is paused, draw the ui, and run the render system only.
    // Note that this systems update has to be ran in the order of what the original systems
    // are ran in
    registry->UpdateSystem(dt, input, "ui");
    registry->UpdateSystem(dt, input, "render");
  }

  frameCount++;
}

void Lucid::InitializeArchetypes()
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

void Lucid::InitializeBuiltInEntities()
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

void Lucid::InitializeBuiltInSystems()
{
  // LucidSystem needs to be first, as this will be what 'calls' the other systems.
  registry->RegisterSystem(new LucidSystem(), "lucid");

  // TODO : Figure this out
  // UiSystem can either be before or after rendersystem.
  // It does somewhat make sense that the uisystem will be after the render system
  // going by the logic of :
  // renderSystem generates a frameBuffer object that the uiSystem can immediately use after
  // But I also recall that I had a specific reason for setting UiSystem to be first
  // perhaps double buffering...?
  registry->RegisterSystem(new UiSystem(), "ui");

  // Demo start -- TODO : This should be separated, need a way of prioritising systems
  registry->RegisterSystem(new PlayerSystem(), "player");
  registry->RegisterSystem(new AiSystem(), "ai");
  // Demo end

  registry->RegisterSystem(new PhysicsSystem(), "physics");

  // Demo start -- PongSystem will need to deal with collision for the ball
  registry->RegisterSystem(new PongSystem(), "pong");
  // Demo end

  registry->RegisterSystem(new RenderSystem(registry), "render");
  registry->RegisterSystem(new AudioSystem(), "audio");
}

void Lucid::InitializeModelEntities()
{
  /*
    Entity modelID = registry->GetAvailableEntityId();
    Entity modelID2 = registry->GetAvailableEntityId();
    Entity modelID3 = registry->GetAvailableEntityId();

    registry->CreateEntity<Model, Transform>(modelID);
    registry->CreateEntity<Model, Transform>(modelID2);
    registry->CreateEntity<Model, Transform>(modelID3);

    registry->AddComponentData<Model>(modelID, Model(MICROPHONE_MODEL));
    registry->AddComponentData<Model>(modelID2, Model(SCIFIHELMET_MODEL));
    registry->AddComponentData<Model>(modelID3, Model(AVOCADO_MODEL));

    registry->AddComponentData<Transform>(modelID, {
                                                       {3.0f, 3.0f, 3.0f},  // position
                                                       {0.0f, 0.0f, 0.0f},  // rotation
                                                       {1.0f, 1.0f, 1.0f},  // scale
                                                   });
    registry->AddComponentData<Transform>(modelID2, {
                                                        {6.0f, 6.0f, 6.0f},  // position
                                                        {0.0f, 0.0f, 0.0f},  // rotation
                                                        {1.0f, 1.0f, 1.0f},  // scale
                                                    });
    registry->AddComponentData<Transform>(modelID3, {
                                                        {1.0f, 1.0f, 1.0f},  // position
                                                        {0.0f, 0.0f, 0.0f},  // rotation
                                                        {1.0f, 1.0f, 1.0f},  // scale
                                                    });
  */

  Entity polyBirdID = registry->GetAvailableEntityId();
  registry->CreateEntity<Model, Transform>(polyBirdID);
  registry->AddComponentData<Model>(polyBirdID, Model(POLYBIRD_MODEL));

  // Move the bird up 3 world units
  registry->GetComponent<Transform>(polyBirdID)->position = {0.0f, 3.0f, 0.0f};
}

void Lucid::InitializeSystems()
{
}

void Lucid::InitializeManyCubes()
{
  // srand(time(NULL));
  for (size_t i = 1; i < 1500; i++)
  {
    Entity cubeID = registry->GetAvailableEntityId();
    registry->CreateEntity<Cube, Transform, RigidBody, ColliderCube>(cubeID);

    Transform* transform = registry->GetComponent<Transform>(cubeID);
    transform->position = {rand() % 100, rand() % 100, rand() % 100};

    // normalize values for the fun of it
    Cube* cube = registry->GetComponent<Cube>(cubeID);
    cube->color = {0.5, 0.5, 0.5};
  }
}

void Lucid::InitializeDemoPongEntities()
{
  // Cube / rectangle entities
  Entity playerPaddleID = registry->GetAvailableEntityId();
  Entity aiPaddleID = registry->GetAvailableEntityId();
  // The ball that will be used to be passed around
  Entity ballID = registry->GetAvailableEntityId();
  Entity pongRulesID = registry->GetAvailableEntityId();

  // Note to add back collidercube back to playerpaddle after addcomponent debugging
  registry->CreateEntity<Cube, Transform, RigidBody>(playerPaddleID);
  registry->CreateEntity<Cube, Transform, RigidBody, ColliderCube>(aiPaddleID);
  registry->CreateEntity<Sphere, Transform, RigidBody, ColliderCube>(ballID);
  registry->CreateEntity<PongRules>(pongRulesID);

  PongRules& pongRules = registry->GetComponent<PongRules>();
  pongRules.playerPaddleID = playerPaddleID;
  pongRules.aiPaddleID = aiPaddleID;
  pongRules.ballID = ballID;

  Transform* playerTransform = registry->GetComponent<Transform>(playerPaddleID);
  Transform* aiTransform = registry->GetComponent<Transform>(aiPaddleID);
  Transform* ballTransform = registry->GetComponent<Transform>(ballID);

  // TODO : registry->GetComponent<Transform> should return a reference not a pointer
  // Move around the transforms of each
  playerTransform->position = {-10.0f, 10.0f, -10.0f};
  ballTransform->position = {0.0f, 10.0f, -10.0f};
  aiTransform->position = {10.0f, 10.0f, 0.0f};

  RigidBody* ballRigidBody = registry->GetComponent<RigidBody>(ballID);
  // note 0.05f is just harded 'movement speed'
  ballRigidBody->velocity =
      glm::normalize(playerTransform->position - ballTransform->position) * 0.020f;

  // Entity platformID = registry->GetAvailableEntityId();
  // registry->CreateEntity<Cube, Transform, RigidBody, ColliderCube>(platformID);

  // // Change the colour so that it is more immediately visible
  // registry->GetComponent<Cube>(platformID)->color = {0.5f, 0.0f, 0.0f};

  // // Set that this platform does not have gravity apply on it.
  // registry->GetComponent<RigidBody>(platformID)->applyGravity = false;

  // Transform* platformTransform = registry->GetComponent<Transform>(platformID);
  // platformTransform->position = {0.0f, 1.0f, -10.0f};
  // platformTransform->scale = {20.0f, 1.0f, 20.0f};


  // Testing purposes
  // registry->AddComponentTest<Test>(playerPaddleID);

  // registry->GetComponentsIter<Test>()->Each(
  //     [&](Test& test) { std::cout << "hello from loop" << std::endl; });

  // Entity soundEffectID = registry->GetAvailableEntityId();
  // Entity musicID = registry->GetAvailableEntityId();

  // registry->CreateEntity<SoundEffect, Transform>(soundEffectID);
  // registry->CreateEntity<Music>(musicID);

  // Transform* soundEffectTransform = registry->GetComponent<Transform>(soundEffectID);
  // soundEffectTransform->position = {0, 0, 0};

  // SoundEffect* soundEffect = registry->GetComponent<SoundEffect>(soundEffectID);
  // soundEffect->filePath = GRUNT_SOUND;

  // Music* music = registry->GetComponent<Music>(musicID);
  // music->filePath = PIANO_MUSIC;
}

void Lucid::InitializeDemoPongSystems()
{
}

void Lucid::InitializeSceneGridLines()
{
  Entity xLineID = registry->GetAvailableEntityId();
  Entity yLineID = registry->GetAvailableEntityId();
  Entity zLineID = registry->GetAvailableEntityId();

  registry->CreateEntity<GridLine, Transform>(xLineID);
  registry->CreateEntity<GridLine, Transform>(yLineID);
  registry->CreateEntity<GridLine, Transform>(zLineID);

  GridLine* xLine = registry->GetComponent<GridLine>(xLineID);
  GridLine* yLine = registry->GetComponent<GridLine>(yLineID);
  GridLine* zLine = registry->GetComponent<GridLine>(zLineID);

  // TODO : Either define maximum limit for a scene or find a compile time number for this
  // Note that if the number is too big, e.g. 100000.0f,
  // opengl will start cutting off some numbers? the lines will not look
  // the same size. Better to stick with a smaller number
  const float MAX_WORLD_COORDINATE_LIMIT = 100.0f;

  xLine->line.color = {0.8f, 0.0f, 0.0f};
  xLine->line.origin.x = -MAX_WORLD_COORDINATE_LIMIT;
  xLine->line.destination.x = MAX_WORLD_COORDINATE_LIMIT;

  yLine->line.color = {0.0f, 0.8f, 0.0f};
  yLine->line.origin.y = -MAX_WORLD_COORDINATE_LIMIT;
  yLine->line.destination.y = MAX_WORLD_COORDINATE_LIMIT;

  zLine->line.color = {0.0f, 0.0f, 0.8f};
  zLine->line.origin.z = -MAX_WORLD_COORDINATE_LIMIT;
  zLine->line.destination.z = MAX_WORLD_COORDINATE_LIMIT;

  // Draw the x-z grid lines
  for (int i = -100; i < 100; i++)
  {
    Entity lineID = registry->GetAvailableEntityId();
    registry->CreateEntity<GridLine, Transform>(lineID);

    GridLine* line = registry->GetComponent<GridLine>(lineID);
    line->line.color = {0.2f, 0.2f, 0.2f};

    line->line.origin.x = static_cast<float>(i);
    line->line.origin.z = -MAX_WORLD_COORDINATE_LIMIT;

    line->line.destination.x = static_cast<float>(i);
    line->line.destination.z = MAX_WORLD_COORDINATE_LIMIT;
  }

  // Draw the z-x grid lines
  for (int i = -100; i < 100; i++)
  {
    Entity lineID = registry->GetAvailableEntityId();
    registry->CreateEntity<GridLine, Transform>(lineID);

    GridLine* line = registry->GetComponent<GridLine>(lineID);
    line->line.color = {0.2f, 0.2f, 0.2f};

    line->line.origin.x = -MAX_WORLD_COORDINATE_LIMIT;
    line->line.origin.z = static_cast<float>(i);

    line->line.destination.x = MAX_WORLD_COORDINATE_LIMIT;
    line->line.destination.z = static_cast<float>(i);
  }
}

void Lucid::SetMouseCallback(std::function<void(GLFWwindow*, int, int, int)> fn)
{
  mouseCallback = fn;
}

void Lucid::SetKeyCallback(std::function<void(GLFWwindow*, int, int, int, int)> fn)
{
  keyCallback = fn;
}

void Lucid::SetScrollCallback(std::function<void(GLFWwindow*, double, double)> fn)
{
  scrollCallback = fn;
}

void Lucid::HandleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  Lucid* lucid = (Lucid*)glfwGetWindowUserPointer(window);

  // Dont handle unknown keys
  if (key == GLFW_KEY_UNKNOWN) return;

  if (action == GLFW_PRESS || action == GLFW_REPEAT)
  {
    lucid->input->keys[key] = true;
  }
  else if (action == GLFW_RELEASE)
  {
    lucid->input->keys[key] = false;
  }
}

void Lucid::HandleMouseCallback(GLFWwindow* window, int button, int action, int mods)
{
  Lucid* lucid = (Lucid*)glfwGetWindowUserPointer(window);

  if (action == GLFW_PRESS)
  {
    if (lucid->input->mouseKeys[button] == false)
    {
      lucid->input->mouseKeys[button] = true;
      lucid->input->lastX = lucid->input->GetMouseX();
      lucid->input->lastY = lucid->input->GetMouseY();
    }
  }
  else if (action == GLFW_RELEASE)
  {
    lucid->input->mouseKeys[button] = false;
  }
}

void Lucid::HandleScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
  /*
   * Only have to care about the yOffset
   *  1 : scrolled up
   * -1 : scrolled down
   */
  Lucid* lucid = (Lucid*)glfwGetWindowUserPointer(window);
  lucid->input->scroll = static_cast<int>(yOffset);
}

void Lucid::MouseCallback(GLFWwindow* window, int button, int action, int mods)
{
  Lucid* lucid = (Lucid*)glfwGetWindowUserPointer(window);
  lucid->mouseCallback(window, button, action, mods);
}

void Lucid::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  // Note that this is basically just a debugging function
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  Lucid* lucid = (Lucid*)glfwGetWindowUserPointer(window);
  lucid->keyCallback(window, key, scancode, action, mods);
}

void Lucid::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
  Lucid* lucid = (Lucid*)glfwGetWindowUserPointer(window);
  lucid->HandleScrollCallback(window, xoffset, yoffset);
}