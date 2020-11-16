#include "lucid.h"

Lucid::Lucid(Registry* registry, Input* input, GLFWwindow* window) {
  Lucid::registry = registry;
  Lucid::input = input;
  Lucid::window = window;

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

  InitializeEntities();
  InitializeSystems();
}

Lucid::~Lucid() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
};

void Lucid::Update(double dt) {
  registry->UpdateSystems(dt, input);
}

void Lucid::InitializeEntities() {
  registry->RegisterArchetype<Model, Transform>();
  registry->RegisterArchetype<ShaderResource>();
  registry->RegisterArchetype<SceneRender>();
  registry->RegisterArchetype<Cube, Transform>();
  registry->RegisterArchetype<Sphere, Transform>();

  uint32_t modelID = registry->GetAvailableEntityId();
  uint32_t modelID2 = registry->GetAvailableEntityId();
  uint32_t modelID3 = registry->GetAvailableEntityId();

  uint32_t cubeID = registry->GetAvailableEntityId();
  uint32_t shaderResourceID = registry->GetAvailableEntityId();
  uint32_t sceneRenderID = registry->GetAvailableEntityId();
  uint32_t sphereID = registry->GetAvailableEntityId();

  registry->CreateEntity<Model, Transform>(modelID);
  registry->CreateEntity<Model, Transform>(modelID2);
  registry->CreateEntity<Model, Transform>(modelID3);
  registry->CreateEntity<Cube, Transform>(cubeID);
  registry->CreateEntity<Sphere, Transform>(sphereID);

  registry->CreateEntity<ShaderResource>(shaderResourceID);
  registry->CreateEntity<SceneRender>(sceneRenderID);

  registry->AddComponentData<Model>(modelID, Model(MICROPHONE_MODEL));
  registry->AddComponentData<Model>(modelID2, Model(SCIFIHELMET_MODEL));
  registry->AddComponentData<Model>(modelID3, Model(AVOCADO_MODEL));

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
                                                    {-2.0f, -2.0f, -2.0f},  // position
                                                    {0.0f, 0.0f, 0.0f},     // rotation
                                                    {2.0f, 2.0f, 2.0f},     // scale
                                                });

  ShaderResource& shaderResource = registry->GetComponent<ShaderResource>();
  shaderResource.modelShader.CreateShader(MODEL_VERTEX_SHADER, MODEL_FRAGMENT_SHADER);
  shaderResource.triangleShader.CreateShader(TRIANGLE_VERTEX_SHADER, TRIANGLE_FRAGMENT_SHADER);
  shaderResource.cubeShader.CreateShader(CUBE_VERTEX_SHADER, CUBE_FRAGMENT_SHADER);
}

void Lucid::InitializeSystems() {
  registry->RegisterSystem(new UiSystem());
  registry->RegisterSystem(new RenderSystem());
  registry->RegisterSystem(new LucidSystem());
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