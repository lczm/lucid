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
  registry->RegisterArchetype<Shader>();
  registry->RegisterArchetype<TextureID>();
  registry->RegisterArchetype<SoundEffect>();
  registry->RegisterArchetype<Music>();

  uint32_t modelID = registry->GetAvailableEntityId();
  uint32_t modelID2 = registry->GetAvailableEntityId();
  uint32_t modelID3 = registry->GetAvailableEntityId();
  uint32_t modelID4 = registry->GetAvailableEntityId();
  uint32_t soundEffectID = registry->GetAvailableEntityId();
  uint32_t musicID = registry->GetAvailableEntityId();
  
  registry->CreateEntity<SoundEffect>(soundEffectID);
  registry->CreateEntity<Music>(musicID);
  registry->CreateEntity<Model, Transform>(modelID);
  registry->CreateEntity<Model, Transform>(modelID2);
  registry->CreateEntity<Model, Transform>(modelID3);
  registry->CreateEntity<Model, Transform>(modelID4);

  registry->AddComponentData<Model>(modelID, Model(MICROPHONE_MODEL));
  registry->AddComponentData<Model>(modelID2, Model(SCIFIHELMET_MODEL));
  registry->AddComponentData<Model>(modelID3, Model(AVOCADO_MODEL));
  registry->AddComponentData<Model>(modelID4, Model(MICROPHONE_MODEL));

  SoundEffect* soundEffect = registry->GetComponent<SoundEffect>(soundEffectID);
  soundEffect->filePath = GRUNT_SOUND;
  Music* music = registry->GetComponent<Music>(musicID);
  music->filePath = PIANO_MUSIC;

  Model* model = registry->GetComponent<Model>(modelID3);
  model->SetTag("avocado");

  uint32_t shaderID = registry->GetAvailableEntityId();
  registry->CreateEntity<Shader>(shaderID);

  Shader* shader = registry->GetComponent<Shader>(shaderID);
  shader->CreateShader(MODEL_VERTEX_SHADER, MODEL_FRAGMENT_SHADER);

  TextureID textureID = registry->GetAvailableEntityId();
  registry->CreateEntity<TextureID>(textureID);
}

void Lucid::InitializeSystems() {
  registry->RegisterSystem(new UiSystem());
  registry->RegisterSystem(new RenderSystem());
  registry->RegisterSystem(new LucidSystem());
  registry->RegisterSystem(new AudioSystem());
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