#include "lucid.h"

Lucid::Lucid(Registry* registry, Input* input, GLFWwindow* window) {
  Lucid::registry = registry;
  Lucid::input = input;
  Lucid::window = window;

  Lucid::yaw = 0.0f;
  Lucid::pitch = 0.0f;
  Lucid::lastX = 0;
  Lucid::lastY = 0;

  for (bool& key : keys) {
    key = false;
  }
  for (bool& mouseKey : mouseKeys) {
    mouseKey = false;
  }

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

  // cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
  // cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

  cameraFront = front;
  cameraPos = glm::normalize(glm::cross(cameraFront, cameraUp));
  cameraUp = glm::normalize(glm::cross(cameraPos, cameraFront));

  registry->RegisterSystem(new RenderSystem());
  registry->RegisterSystem(new LucidSystem());

  registry->RegisterArchetype<Model>();
  registry->RegisterArchetype<Shader>();

  uint32_t modelID = registry->GetAvailableEntityId();
  uint32_t modelID2 = registry->GetAvailableEntityId();
  uint32_t modelID3 = registry->GetAvailableEntityId();
  uint32_t modelID4 = registry->GetAvailableEntityId();

  registry->CreateEntity<Model>(modelID);
  registry->CreateEntity<Model>(modelID2);
  registry->CreateEntity<Model>(modelID3);
  registry->CreateEntity<Model>(modelID4);

  registry->AddComponentData<Model>(modelID, Model(MICROPHONE_MODEL));
  registry->AddComponentData<Model>(modelID2, Model(SCIFIHELMET_MODEL));
  registry->AddComponentData<Model>(modelID3, Model(AVOCADO_MODEL));
  registry->AddComponentData<Model>(modelID4, Model(MICROPHONE_MODEL));

  Model* model = registry->GetComponent<Model>(modelID3);
  model->SetTag("avocado");

  uint32_t shaderID = registry->GetAvailableEntityId();
  registry->CreateEntity<Shader>(shaderID);

  modelShaderID = shaderID;

  Shader* shader = registry->GetComponent<Shader>(shaderID);
  shader->CreateShader(MODEL_VERTEX_SHADER, MODEL_FRAGMENT_SHADER);
}

Lucid::~Lucid() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
};

void Lucid::Update(double dt) {
  HandleMousePan(dt);
  HandleKeyboardPan(dt);

  view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

  glm::mat4 projection = glm::perspective(
      glm::radians(45.0f), static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT),
      0.1f, 100.0f);

  Shader* shader = registry->GetComponent<Shader>(modelShaderID);

  shader->Bind();
  shader->SetUniformMatFloat4("projection", projection);
  shader->SetUniformMatFloat4("view", view);
  shader->Unbind();

  registry->UpdateSystems(dt, input);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::Begin("Lucid");
  ImGui::BeginTabBar("Lucid Tab Bar");

  ImGui::EndTabBar();
  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool Lucid::IsKeyDown(int key) {
  // This sets everything to capitalised ascii numbers
  // lets us use something like
  // input->isKeyDown('l')
  // input->isKeyDown('L')
  // I guess in the future if we need to do mod keys, this will have to be
  // changed
  if (key >= 97) {
    key -= 32;
  }
  return keys[key];
}

void Lucid::UpdateCameraVector(float xOffset, float yOffset) {
  xOffset *= CAMERA_SENSITIVITY;
  yOffset *= CAMERA_SENSITIVITY;

  yaw += xOffset;
  pitch += yOffset;

  if (pitch > 89.0f) pitch = 89.0f;
  if (pitch < -89.0f) pitch = -89.0f;

  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  cameraFront = glm::normalize(front);
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
    lucid->keys[key] = true;
  } else if (action == GLFW_RELEASE) {
    lucid->keys[key] = false;
  }
}

void Lucid::HandleMouseCallback(GLFWwindow* window, int button, int action, int mods) {
  Lucid* lucid = (Lucid*)glfwGetWindowUserPointer(window);

  if (action == GLFW_PRESS) {
    if (lucid->mouseKeys[button] == false) {
      lucid->mouseKeys[button] = true;
      lucid->lastX = lucid->input->GetMouseX();
      lucid->lastY = lucid->input->GetMouseY();
    }
  } else if (action == GLFW_RELEASE) {
    lucid->mouseKeys[button] = false;
  }
}

void Lucid::HandleScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
  /*
   * Only have to care about the yOffset
   *  1 : scrolled up
   * -1 : scrolled down
   */
  Lucid* lucid = (Lucid*)glfwGetWindowUserPointer(window);
  lucid->scroll = yOffset;
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

void Lucid::HandleMousePan(double dt) {
  if (mouseKeys[MOUSE_LEFT] && (lastX != input->GetMouseX() || lastY != input->GetMouseY())) {
    float offsetX = input->GetMouseX() - lastX;
    float offsetY = input->GetMouseY() - lastY;

    // Handle the x-axis movement
    cameraPos -=
        glm::normalize(glm::cross(cameraFront, cameraUp)) * static_cast<float>(offsetX * dt);

    // Handle the y-axis movement
    cameraPos -= glm::normalize(cameraUp) * static_cast<float>(offsetY * dt);

    lastX = input->GetMouseX();
    lastY = input->GetMouseY();
  } else if (mouseKeys[MOUSE_RIGHT] &&
             (lastX != input->GetMouseX() || lastY != input->GetMouseY())) {
    float offsetX = input->GetMouseX() - lastX;
    float offsetY = input->GetMouseY() - lastY;

    lastX = input->GetMouseX();
    lastY = input->GetMouseY();

    UpdateCameraVector(offsetX, offsetY);
  }

  // Scroll up
  if (scroll == 1) {
    cameraPos += static_cast<float>(SCROLL_SPEED * dt) * cameraFront;
  }

  // Scroll down
  if (scroll == -1) {
    cameraPos -= static_cast<float>(SCROLL_SPEED * dt) * cameraFront;
  }

  // Reset the scroll variable once done
  scroll = 0;
}

void Lucid::HandleKeyboardPan(double dt) {
  if (IsKeyDown('w')) {
    cameraPos += static_cast<float>(CAMERA_SPEED * dt) * cameraFront;
  }

  if (IsKeyDown('s')) {
    cameraPos -= static_cast<float>(CAMERA_SPEED * dt) * cameraFront;
  }

  if (IsKeyDown('a')) {
    cameraPos -=
        glm::normalize(glm::cross(cameraFront, cameraUp)) * static_cast<float>(CAMERA_SPEED * dt);
  }

  if (IsKeyDown('d')) {
    cameraPos +=
        glm::normalize(glm::cross(cameraFront, cameraUp)) * static_cast<float>(CAMERA_SPEED * dt);
  }
}
