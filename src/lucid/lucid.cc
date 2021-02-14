#include "lucid.h"

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

  // Engine startup
  InitializeEngineComponents();

  // System startup, retrieved from startup.h
  InitSystems(registry);

  InitializeModelEntities();

#if DEBUG
  InitializeSceneGridLines();
#endif

  registry->RegisterEmptyArchetype();

  // TODO : This should be abstracted out into a user system
  // Demo pong
  // InitializeDemoPongEntities();
  // InitializeDemoPongSystems();

  // Move scene camera
  // registry->GetEditorResource<Camera>().TranslateInWorld({0.0f, 1.0f, 20.0f});

  // Move game camera
  // registry->GetResource<Camera>().TranslateInWorld({0.0f, 1.0f, 5.0f});

  DevDebug& devDebug = registry->GetEditorResource<DevDebug>();

  // Create scene camera
  Entity sceneCameraId = registry->GetAvailableEntityId();
  // Scene
  registry->CreateEntity<Transform, Camera>(sceneCameraId);
  // Set where the camera is in the scene. (Behind origin point by a little bit)
  registry->GetComponent<Transform>(sceneCameraId).position = {0.0f, -1.0f, -20.0f};
  // Add scene camera id to devDebug
  devDebug.sceneCameraId = sceneCameraId;

  // Create game camera
  Entity gameCameraId = registry->GetAvailableEntityId();
  // Game camera requires a model for the user to 'move'
  // registry->CreateEntity<Model, Transform, Camera>(gameCameraId);
  // registry->AddComponentData<Model>(gameCameraId, Model(CAMERA_MODEL, registry));
  // registry->GetComponent<Transform>(gameCameraId).position = {0.0f, 1.0f, 15.0f};

  registry->CreateEntity<Transform, Camera>(gameCameraId);
  registry->GetComponent<Transform>(gameCameraId).position = {0.0f, -1.0f, -15.0f};

  // registry->GetComponent<Transform>(gameCameraId).rotation =
  //     RotateQuatY(registry->GetComponent<Transform>(gameCameraId).rotation, glm::radians(90.0f));

  // registry->GetComponent<Transform>(gameCameraId).rotation =
  //     RotateQuatX(registry->GetComponent<Transform>(gameCameraId).rotation,
  //     glm::radians(-90.0f));

  // registry->GetComponent<Transform>(gameCameraId).scale /= 80.0f;
  // Add game camera id to devDebug
  devDebug.gameCameraId = gameCameraId;

// Upon initializing the engine, serialize everything in.
// This can only be ran when it is ran through -DRELEASE=1
// Because this is going to assume that "data.json" exists.
#if RELEASE
  // Workspace& workspace = registry->GetEditorResource<Workspace>();
  // std::string workspaceRoot = "../" + workspace.relativeProjectRoot->path.string();
  // std::string workspaceRootData = workspaceRoot + "/data.json";
  // std::string workspaceRootDataConverted = ConvertFsToNativePaths(workspaceRootData);

  // std::cout << "Trying to serialize in workspaceRootDataConverted" << std::endl;
  // std::cout << workspaceRootDataConverted << std::endl;
  // std::cout << "Trying to serialize in workspaceRootDataConverted END" << std::endl;

  // SerializeAllIn(registry, workspaceRootDataConverted);
  SerializeAllIn(registry, "data.json");
#endif
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
    DevDebug& devDebug = registry->GetEditorResource<DevDebug>();
    devDebug.fps = frameCount;

    secondDt -= 1.0f;
    frameCount = 0;
  }

  // Get the current game state
  GameEngineState& gameEngineState = registry->GetEditorResource<GameEngineState>();

#if DEBUG
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
#endif

#if RELEASE
  registry->UpdateSystems(dt, input);
#endif

  frameCount++;
}

void Lucid::InitializeModelEntities()
{
  {
    // Entity kingBooId = registry->GetAvailableEntityId();
    // registry->CreateEntity<Model, Transform, ColliderCube, Sound>(kingBooId);
    // registry->AddComponentData<Model>(kingBooId, Model(KING_BOO_MODEL, registry));
    // registry->AddComponentData<Sound>(kingBooId, Sound(PIANO_MUSIC));
    // registry->GetComponent<Model>(kingBooId).toAnimate = true;
    // registry->GetComponent<Transform>(kingBooId).position = {0.0f, 0.0f, 0.0f};
    // registry->GetComponent<Transform>(kingBooId).scale /= 150.0f;

    // registry->GetComponent<ColliderPolygon>(kingBooId).SetVertices(
    //    registry->GetComponent<Model>(kingBooId).vertices);
    // registry->GetComponent<ColliderCube>(kingBooId).SetVertices(
    //     GetBoundingBoxVertices(registry->GetComponent<Model>(kingBooId).boundingBox));
  }

  // {
  //   Entity happySharkId = registry->GetAvailableEntityId();
  //   registry->CreateEntity<Model, Transform>(happySharkId);
  //   registry->AddComponentData<Model>(happySharkId, Model(HAPPY_SHARK_MODEL, registry));
  //   registry->GetComponent<Model>(happySharkId).toAnimate = true;
  //   registry->GetComponent<Transform>(happySharkId).position = {0.0f, 10.0f, 0.0f};
  //   registry->GetComponent<Transform>(happySharkId).scale /= 100.0f;
  // }

  // {
  //   Entity cheeseCakeId = registry->GetAvailableEntityId();
  //   registry->CreateEntity<Model, Transform>(cheeseCakeId);
  //   registry->AddComponentData<Model>(cheeseCakeId, Model(CHEESE_CAKE_MODEL, registry));
  //   registry->GetComponent<Model>(cheeseCakeId).toAnimate = true;
  //   registry->GetComponent<Transform>(cheeseCakeId).position = {-3.0f, 0.0f, 0.0f};
  //   registry->GetComponent<Transform>(cheeseCakeId).scale /= 150.0f;
  // }

  // // In-game camera
  // {
  //   Entity cameraId = registry->GetAvailableEntityId();
  //   registry->CreateEntity<Model, Transform>(cameraId);
  //   registry->AddComponentData<Model>(cameraId, Model(CAMERA_MODEL, registry));
  //   registry->GetComponent<Transform>(cameraId).position = {0.0f, 0.0f, 15.0f};
  //   registry->GetComponent<Transform>(cameraId).scale /= 80.0f;
  //   // TODO : Rotate the camera to be correct
  //   // registry->GetComponent<Transform>(cameraId).rotation = RotateQuat()
  //   // RotateTransform(registry->GetComponent<Transform>(cameraId), 90.0f, {1.0f, 0.0f, 0.0f});
  // }
}

void Lucid::InitializeEngineComponents()
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
  // registry->CreateEditorResource<Camera>();
  // In Game Camera Model
  // registry->CreateEditorResource<Model>();

  // Resources are for the users to use
  registry->CreateResource<RigidBodyConfiguration>();
  registry->CreateResource<ShaderResource>();
  // Game Camera
  // registry->CreateResource<Camera>();

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

void Lucid::InitializeDemoPongEntities()
{
  // // Cube / rectangle entities
  // Entity playerPaddleID = registry->GetAvailableEntityId();
  // Entity aiPaddleID = registry->GetAvailableEntityId();
  // // The ball that will be used to be passed around
  // Entity ballID = registry->GetAvailableEntityId();

  // // Note to add back collidercube back to playerpaddle after addcomponent debugging
  // registry->CreateEntity<Cube, Transform, RigidBody, ColliderCube>(playerPaddleID);
  // registry->CreateEntity<Cube, Transform, RigidBody, ColliderCube>(aiPaddleID);
  // registry->CreateEntity<Sphere, Transform, RigidBody, ColliderSphere>(ballID);

  // Transform& playerTransform = registry->GetComponent<Transform>(playerPaddleID);
  // Transform& playerColliderTransform =
  //     registry->GetComponent<ColliderCube>(playerPaddleID).transform;
  // playerColliderTransform.scale *= 1.2;

  // Transform& aiTransform = registry->GetComponent<Transform>(aiPaddleID);
  // Transform& aiColliderTransform = registry->GetComponent<ColliderCube>(aiPaddleID).transform;
  // aiColliderTransform.scale *= 1.2;

  // Transform& ballTransform = registry->GetComponent<Transform>(ballID);

  // // TODO : registry->GetComponent<Transform> should return a reference not a pointer
  // // Move around the transforms of each
  // playerTransform.position = {-10.0f, 5.0f, -10.0f};
  // ballTransform.position = {0.0f, 5.0f, -10.0f};
  // aiTransform.position = {10.0f, 5.0f, 0.0f};

  // RigidBody& ballRigidBody = registry->GetComponent<RigidBody>(ballID);
  // ballRigidBody.velocity =
  //     glm::normalize(playerTransform.position - ballTransform.position) * 0.020f;

  // registry->GetComponent<RigidBody>(playerPaddleID).applyGravity = false;
  // registry->GetComponent<RigidBody>(aiPaddleID).applyGravity = false;
  // registry->GetComponent<RigidBody>(ballID).applyGravity = false;
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

  GridLine& xLine = registry->GetComponent<GridLine>(xLineID);
  GridLine& yLine = registry->GetComponent<GridLine>(yLineID);
  GridLine& zLine = registry->GetComponent<GridLine>(zLineID);

  // TODO : Either define maximum limit for a scene or find a compile time number for this
  // Note that if the number is too big, e.g. 100000.0f,
  // opengl will start cutting off some numbers? the lines will not look
  // the same size. Better to stick with a smaller number
  const float MAX_WORLD_COORDINATE_LIMIT = 100.0f;

  xLine.line.color = {0.8f, 0.0f, 0.0f};
  xLine.line.origin.x = -MAX_WORLD_COORDINATE_LIMIT;
  xLine.line.destination.x = MAX_WORLD_COORDINATE_LIMIT;

  yLine.line.color = {0.0f, 0.8f, 0.0f};
  yLine.line.origin.y = -MAX_WORLD_COORDINATE_LIMIT;
  yLine.line.destination.y = MAX_WORLD_COORDINATE_LIMIT;

  zLine.line.color = {0.0f, 0.0f, 0.8f};
  zLine.line.origin.z = -MAX_WORLD_COORDINATE_LIMIT;
  zLine.line.destination.z = MAX_WORLD_COORDINATE_LIMIT;

  // Draw the x-z grid lines
  for (int i = -100; i < 100; i++)
  {
    Entity lineID = registry->GetAvailableEntityId();
    registry->CreateEntity<GridLine, Transform>(lineID);

    GridLine& line = registry->GetComponent<GridLine>(lineID);
    line.line.color = {0.2f, 0.2f, 0.2f};

    line.line.origin.x = static_cast<float>(i);
    line.line.origin.z = -MAX_WORLD_COORDINATE_LIMIT;

    line.line.destination.x = static_cast<float>(i);
    line.line.destination.z = MAX_WORLD_COORDINATE_LIMIT;
  }

  // Draw the z-x grid lines
  for (int i = -100; i < 100; i++)
  {
    Entity lineID = registry->GetAvailableEntityId();
    registry->CreateEntity<GridLine, Transform>(lineID);

    GridLine& line = registry->GetComponent<GridLine>(lineID);
    line.line.color = {0.2f, 0.2f, 0.2f};

    line.line.origin.x = -MAX_WORLD_COORDINATE_LIMIT;
    line.line.origin.z = static_cast<float>(i);

    line.line.destination.x = MAX_WORLD_COORDINATE_LIMIT;
    line.line.destination.z = static_cast<float>(i);
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