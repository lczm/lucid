#include "lucid.h"

Lucid::Lucid(Registry* registry, Input* input, GLFWwindow* window) {
  Lucid::registry = registry;
  Lucid::input = input;
  Lucid::window = window;

  Lucid::firstMouse = true;
  for (bool& key : keys) {
    key = false;
  }
  for (bool& mouseKey : mouseKeys) {
    mouseKey = false;
  }

  Lucid::yaw = 0.0f;
  Lucid::pitch = 0.0f;
  Lucid::lastX = 0;
  Lucid::lastY = 0;

  modelShader.CreateShader(MODEL_VERTEX_SHADER, MODEL_FRAGMENT_SHADER);

  // Lucid::microphone = new Model(MICROPHONE_MODEL);
  // Lucid::helmet = new Model(SCIFIHELMET_MODEL);
  // Lucid::avocado = new Model(AVOCADO_MODEL);

  // Target this window for user pointer for GLFW, this is so that
  // in callbacks, we can retrieve back the class
  glfwSetWindowUserPointer(window, this);

  // Set callbacks
  glfwSetKeyCallback(window, KeyCallback);
  glfwSetMouseButtonCallback(window, MouseCallback);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;

  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(GLSL_VERSION);

  // float vertices[] = {
  //     -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  //
  //     0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  //
  //     0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 1.0f,  //
  //     0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 1.0f,  //
  //     -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, 1.0f,  //
  //     -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  //

  //     -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 1.0f,  //
  //     0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, 1.0f,  //
  //     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  //
  //     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  //
  //     -0.5f, 0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  //
  //     -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 1.0f,  //

  //     -0.5f, 0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  //
  //     -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, 1.0f,  //
  //     -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  //
  //     -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  //
  //     -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 1.0f,  //
  //     -0.5f, 0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  //

  //     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  //
  //     0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 1.0f,  //
  //     0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  //
  //     0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  //
  //     0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, 1.0f,  //
  //     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  //

  //     -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  //
  //     0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  //
  //     0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, 1.0f,  //
  //     0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, 1.0f,  //
  //     -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 1.0f,  //
  //     -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  //

  //     -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, 1.0f,  //
  //     0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 1.0f,  //
  //     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  //
  //     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  //
  //     -0.5f, 0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  //
  //     -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, 1.0f,  //
  // };

  // uint32_t indices[] = {
  //     0, 1, 3,  // first triangle
  //     1, 2, 3   // second triangle
  // };

  // glGenVertexArrays(1, &VAO);
  // glBindVertexArray(VAO);

  // glGenBuffers(1, &VBO);
  // glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
  // (void*)0); glEnableVertexAttribArray(0);

  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
  //                       (void*)(3 * sizeof(float)));
  // glEnableVertexAttribArray(1);

  // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
  //                       (void*)(6 * sizeof(float)));
  // glEnableVertexAttribArray(2);

  // shader.CreateShader(TRIANGLE_VERTEX_SHADER, TRIANGLE_FRAGMENT_SHADER);

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

  registry->CreateEntity<Model>(modelID);
  registry->CreateEntity<Model>(modelID2);
  registry->CreateEntity<Model>(modelID3);

  registry->AddComponentData<Model>(modelID, Model(MICROPHONE_MODEL));
  registry->AddComponentData<Model>(modelID2, Model(SCIFIHELMET_MODEL));
  registry->AddComponentData<Model>(modelID3, Model(AVOCADO_MODEL));

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
  if (mouseKeys[MOUSE_LEFT] &&
      (lastX != input->GetMouseX() || lastY != input->GetMouseY())) {
    float offsetX = input->GetMouseX() - lastX;
    float offsetY = input->GetMouseY() - lastY;

    lastX = input->GetMouseX();
    lastY = input->GetMouseY();

    UpdateCameraVector(offsetX, offsetY);
  }

  const float cameraSpeed = 5.0f;
  if (IsKeyDown('w'))
    cameraPos += static_cast<float>(cameraSpeed * dt) * cameraFront;
  if (IsKeyDown('s'))
    cameraPos -= static_cast<float>(cameraSpeed * dt) * cameraFront;
  if (IsKeyDown('a'))
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) *
                 static_cast<float>(cameraSpeed * dt);
  if (IsKeyDown('d'))
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) *
                 static_cast<float>(cameraSpeed * dt);

  // glm::vec3 cubePositions[] = {glm::vec3(0.0f, 0.0f, 0.0f),      //
  //                              glm::vec3(2.0f, 5.0f, -15.0f),    //
  //                              glm::vec3(-1.5f, -2.2f, -2.5f),   //
  //                              glm::vec3(-3.8f, -2.0f, -12.3f),  //
  //                              glm::vec3(2.4f, -0.4f, -3.5f),    //
  //                              glm::vec3(-1.7f, 3.0f, -7.5f),    //
  //                              glm::vec3(1.3f, -2.0f, -2.5f),    //
  //                              glm::vec3(1.5f, 2.0f, -2.5f),     //
  //                              glm::vec3(1.5f, 0.2f, -1.5f),     //
  //                              glm::vec3(-1.3f, 1.0f, -1.5f)};   //

  view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

  glm::mat4 projection = glm::perspective(
      glm::radians(45.0f),
      static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT),
      0.1f, 100.0f);

  Shader* shader = registry->GetComponent<Shader>(modelShaderID);
  shader->Bind();
  shader->SetUniformMatFloat4("projection", projection);
  shader->SetUniformMatFloat4("view", view);

  // modelShader.Bind();

  // modelShader.SetUniformMatFloat4("projection", projection);
  // modelShader.SetUniformMatFloat4("view", view);

  // glm::mat4 model = glm::mat4(1.0f);
  // model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));  // translate it down so it's at the center of the scene
  // model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));  // it's a bit too big for our scene, so scale it down-

  // modelShader.SetUniformMatFloat4("model", model);
  // Lucid::microphone->Draw(modelShader);

  // model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));  // translate it down so it's at the center of the scene
  // modelShader.SetUniformMatFloat4("model", model);
  // Lucid::helmet->Draw(modelShader);

  // Translate & Scale the avocado as the base model itself is very small.
  // model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
  // model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));
  // modelShader.SetUniformMatFloat4("model", model);
  // Lucid::avocado->Draw(modelShader);

  // shader.Bind();
  // shader.SetUniformMatFloat4("view", view);
  // shader.SetUniformMatFloat4("projection", projection);

  // glBindVertexArray(VAO);

  // for (size_t i = 0; i < 10; i++) {
  //   glm::mat4 model = glm::mat4(1.0f);
  //   model = glm::translate(model, cubePositions[i]);

  //   float angle = 20.0f * i;

  //   model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f),
  //                       glm::vec3(0.5f, 1.0f, 0.0f));

  //   shader.SetUniformMatFloat4("model", model);
  //   glDrawArrays(GL_TRIANGLES, 0, 36);
  // }

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

void Lucid::HandleKeyCallback(GLFWwindow* window, int key, int scancode,
                              int action, int mods) {
  // Dont handle unknown keys
  if (key == GLFW_KEY_UNKNOWN) return;

  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    keys[key] = true;
  } else if (action == GLFW_RELEASE) {
    keys[key] = false;
  }
}

void Lucid::HandleMouseCallback(GLFWwindow* window, int button, int action,
                                int mods) {
  if (action == GLFW_PRESS) {
    if (mouseKeys[button] == false) {
      mouseKeys[button] = true;
      lastX = input->GetMouseX();
      lastY = input->GetMouseY();
    }
  } else if (action == GLFW_RELEASE) {
    mouseKeys[button] = false;
  }
}

void Lucid::UpdateCameraVector(float xOffset, float yOffset) {
  const float sensitivity = 0.1f;

  xOffset *= sensitivity;
  yOffset *= sensitivity;

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
