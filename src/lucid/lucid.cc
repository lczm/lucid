#include "lucid.h"

Lucid::Lucid(Registry* registry, Input* input, GLFWwindow* window) {
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

  InitializeBulitInEntities();
  InitializeBuiltInSystems();

  // InitializeEntities();
  // InitializeSystems();

  // TODO : This should be abstracted out into a user system
  // Demo pong
  InitializeDemoPongEntities();
  InitializeDemoPongSystems();
}

Lucid::~Lucid() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
};

void Lucid::Update() {
  auto now = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = now - timer;
  dt = elapsed.count();
  timer = now;

  secondDt += dt;
  frameCount++;

  // If it has been a second
  if (secondDt >= 1.0f) {
    // Update the internal fps counter before resetting it back to 0
    DevDebug& devDebug = registry->GetComponent<DevDebug>();
    devDebug.fps = frameCount;

    secondDt -= 1.0f;
    frameCount = 0;
  }

  registry->UpdateSystems(dt, input);
}

void Lucid::InitializeBulitInEntities() {
  // Builtin usage, not for the user
  // Singletons single struct usage
  registry->RegisterArchetype<ShaderResource>();
  registry->RegisterArchetype<SceneRender>();
  registry->RegisterArchetype<DevDebug>();

  Entity shaderResourceID = registry->GetAvailableEntityId();
  Entity sceneRenderID = registry->GetAvailableEntityId();
  Entity devDebugID = registry->GetAvailableEntityId();

  registry->CreateEntity<ShaderResource>(shaderResourceID);
  registry->CreateEntity<SceneRender>(sceneRenderID);
  registry->CreateEntity<DevDebug>(devDebugID);

  ShaderResource& shaderResource = registry->GetComponent<ShaderResource>();
  shaderResource.modelShader.CreateShader(MODEL_VERTEX_SHADER, MODEL_FRAGMENT_SHADER);
  shaderResource.triangleShader.CreateShader(TRIANGLE_VERTEX_SHADER, TRIANGLE_FRAGMENT_SHADER);
  shaderResource.primitiveShader.CreateShader(PRIMITIVE_VERTEX_SHADER, PRIMITIVE_FRAGMENT_SHADER);

  registry->RegisterArchetype<Line, Transform>();

  Entity xLineID = registry->GetAvailableEntityId();
  Entity yLineID = registry->GetAvailableEntityId();
  Entity zLineID = registry->GetAvailableEntityId();

  registry->CreateEntity<Line, Transform>(xLineID);
  registry->CreateEntity<Line, Transform>(yLineID);
  registry->CreateEntity<Line, Transform>(zLineID);

  Line* xLine = registry->GetComponent<Line>(xLineID);
  Line* yLine = registry->GetComponent<Line>(yLineID);
  Line* zLine = registry->GetComponent<Line>(zLineID);

  // TODO : Either define maximum limit for a scene or find a compile time number for this
  xLine->color = {0.8f, 0.0f, 0.0f};
  xLine->origin.x = -1000000.0f;
  xLine->destination.x = 1000000.0f;

  yLine->color = {0.0f, 0.8f, 0.0f};
  yLine->origin.y = -1000000.0f;
  yLine->destination.y = 1000000.0f;

  zLine->color = {0.0f, 0.0f, 0.8f};
  zLine->origin.z = -1000000.0f;
  zLine->destination.z = 1000000.0f;
}

void Lucid::InitializeBuiltInSystems() {
  // LucidSystem needs to be first, as this will be what 'calls' the other systems.
  registry->RegisterSystem(new LucidSystem());

  // TODO : Figure this out
  // UiSystem can either be before or after rendersystem.
  // It does somewhat make sense that the uisystem will be after the render system
  // going by the logic of :
  // renderSystem generates a frameBuffer object that the uiSystem can immediately use after
  // But I also recall that I had a specific reason for setting UiSystem to be first
  // perhaps double buffering...?
  registry->RegisterSystem(new UiSystem());

  // Demo start -- TODO : This should be separated, need a way of prioritising systems
  registry->RegisterSystem(new PlayerSystem());
  registry->RegisterSystem(new AiSystem());
  registry->RegisterSystem(new MovementSystem());
  // Demo end

  registry->RegisterSystem(new PhysicsSystem());

  // Demo start -- PongSystem will need to deal with collision for the ball
  registry->RegisterSystem(new PongSystem());
  // Demo end

  registry->RegisterSystem(new RenderSystem());
}

void Lucid::InitializeEntities() {
  registry->RegisterArchetype<Model, Transform>();
  registry->RegisterArchetype<Cube, Transform>();
  registry->RegisterArchetype<Sphere, Transform>();

  Entity modelID = registry->GetAvailableEntityId();
  Entity modelID2 = registry->GetAvailableEntityId();
  Entity modelID3 = registry->GetAvailableEntityId();

  Entity cubeID = registry->GetAvailableEntityId();
  Entity sphereID = registry->GetAvailableEntityId();

  registry->CreateEntity<Model, Transform>(modelID);
  registry->CreateEntity<Model, Transform>(modelID2);
  registry->CreateEntity<Model, Transform>(modelID3);
  registry->CreateEntity<Cube, Transform>(cubeID);
  registry->CreateEntity<Sphere, Transform>(sphereID);

  registry->AddComponentData<Model>(modelID, Model(MICROPHONE_MODEL));
  registry->AddComponentData<Model>(modelID2, Model(SCIFIHELMET_MODEL));
  registry->AddComponentData<Model>(modelID3, Model(AVOCADO_MODEL));

  registry->GetComponentsIter<Cube>()->Each([](Cube& cube) { cube.Build(); });

  registry->GetComponentsIter<Sphere, Transform>()->Each([](Sphere& sphere, Transform& transform) {
    sphere.radius = 1.0f;
    sphere.sectors = 36;
    sphere.stacks = 18;
    sphere.BuildSphere();
    sphere.Build();

    transform.position = {-5.0f, -5.0f, -5.0f};
    transform.rotation = {0.0f, 0.0f, 0.0f};
    transform.scale = {1.0f, 1.0f, 1.0f};
  });

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
                                                      {1.0f, 1.0f, 1.0f},     // position
                                                      {0.0f, 0.0f, 0.0f},     // rotation
                                                      {30.0f, 30.0f, 30.0f},  // scale
                                                  });
  registry->AddComponentData<Transform>(cubeID, {
                                                    {0.0f, 0.0f, 0.0f},  // position
                                                    {0.0f, 0.0f, 0.0f},  // rotation
                                                    {2.0f, 2.0f, 2.0f},  // scale
                                                });
}

void Lucid::InitializeSystems() {
}

void Lucid::InitializeDemoPongEntities() {
  // Register the necessary archetypes
  // Note that this should be using `ColliderCube` but it is hardcoded like this for now.
  registry->RegisterArchetype<Cube, Transform, RigidBody, BoundingBoxCube>();
  registry->RegisterArchetype<Sphere, Transform, RigidBody, BoundingBoxCube>();
  registry->RegisterArchetype<PongRules>();

  // This archetype is used for ray casting
  // Don't have to register again as it is already registered once.
  // registry->RegisterArchetype<Line, Transform>();

  Entity rayID = registry->GetAvailableEntityId();
  registry->CreateEntity<Line, Transform>(rayID);

  DevDebug& devDebug = registry->GetComponent<DevDebug>();
  devDebug.rayID = rayID;

  // Cube / rectangle entities
  Entity playerPaddleID = registry->GetAvailableEntityId();
  Entity aiPaddleID = registry->GetAvailableEntityId();
  // The ball that will be used to be passed around
  Entity ballID = registry->GetAvailableEntityId();
  Entity pongRulesID = registry->GetAvailableEntityId();

  registry->CreateEntity<Cube, Transform, RigidBody, BoundingBoxCube>(playerPaddleID);
  registry->CreateEntity<Cube, Transform, RigidBody, BoundingBoxCube>(aiPaddleID);
  registry->CreateEntity<Sphere, Transform, RigidBody, BoundingBoxCube>(ballID);
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
  playerTransform->position = {-10.0f, 0.0f, -10.0f};
  ballTransform->position = {0.0f, 0.0f, -10.0f};
  aiTransform->position = {10.0f, 0.0f, 0.0f};

  RigidBody* ballRigidBody = registry->GetComponent<RigidBody>(ballID);
  // note 0.05f is just harded 'movement speed'
  // ballRigidBody->velocity =
  //     glm::normalize(playerTransform->position - ballTransform->position) * 0.020f;

  // TODO : This can be simplified
  registry->GetComponentsIter<Cube>()->Each([](Cube& cube) { cube.Build(); });

  // TODO : This can be ismplified
  registry->GetComponentsIter<BoundingBoxCube>()->Each(
      [](BoundingBoxCube& boundingBoxCube) { boundingBoxCube.Build(); });

  // TODO : This can be simplified
  registry->GetComponentsIter<Sphere>()->Each([](Sphere& sphere) {
    sphere.radius = 1.0f;
    sphere.sectors = 36;
    sphere.stacks = 18;
    sphere.BuildSphere();
    sphere.Build();
  });

  // TODO : Need to scale the cubes to become 'paddles'
}

void Lucid::InitializeDemoPongSystems() {
}

void Lucid::SetMouseCallback(std::function<void(GLFWwindow*, int, int, int)> fn) {
  mouseCallback = fn;
}

void Lucid::SetKeyCallback(std::function<void(GLFWwindow*, int, int, int, int)> fn) {
  keyCallback = fn;
}

void Lucid::SetScrollCallback(std::function<void(GLFWwindow*, double, double)> fn) {
  scrollCallback = fn;
}

void Lucid::HandleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  Lucid* lucid = (Lucid*)glfwGetWindowUserPointer(window);

  // Dont handle unknown keys
  if (key == GLFW_KEY_UNKNOWN) return;

  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    lucid->input->keys[key] = true;
  } else if (action == GLFW_RELEASE) {
    lucid->input->keys[key] = false;
  }
}

void Lucid::HandleMouseCallback(GLFWwindow* window, int button, int action, int mods) {
  Lucid* lucid = (Lucid*)glfwGetWindowUserPointer(window);

  if (action == GLFW_PRESS) {
    if (lucid->input->mouseKeys[button] == false) {
      lucid->input->mouseKeys[button] = true;
      lucid->input->lastX = lucid->input->GetMouseX();
      lucid->input->lastY = lucid->input->GetMouseY();
    }
  } else if (action == GLFW_RELEASE) {
    lucid->input->mouseKeys[button] = false;
  }
}

void Lucid::HandleScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
  /*
   * Only have to care about the yOffset
   *  1 : scrolled up
   * -1 : scrolled down
   */
  Lucid* lucid = (Lucid*)glfwGetWindowUserPointer(window);
  lucid->input->scroll = yOffset;
}

void Lucid::MouseCallback(GLFWwindow* window, int button, int action, int mods) {
  Lucid* lucid = (Lucid*)glfwGetWindowUserPointer(window);
  lucid->mouseCallback(window, button, action, mods);
}

void Lucid::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  // Note that this is basically just a debugging function
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  Lucid* lucid = (Lucid*)glfwGetWindowUserPointer(window);
  lucid->keyCallback(window, key, scancode, action, mods);
}

void Lucid::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  Lucid* lucid = (Lucid*)glfwGetWindowUserPointer(window);
  lucid->HandleScrollCallback(window, xoffset, yoffset);
}