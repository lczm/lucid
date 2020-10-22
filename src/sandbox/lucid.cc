#include "lucid.h"

Lucid::Lucid(Registry* registry, Input* input, GLFWwindow* window) {
  Lucid::registry = registry;
  Lucid::input = input;
  Lucid::window = window;

  Lucid::firstMouse = true;
  for (size_t i = 0; i < 350; i++) {
    keys[i] = false;
  }

  Lucid::yaw = 0;
  Lucid::pitch = 0;

  // glfwSetCursorPosCallback(window, Lucid::MouseCallback);

  glfwSetWindowUserPointer(window, this);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetKeyCallback(window, KeyCallback);
  glfwSetCursorPosCallback(window, MouseCallback);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;

  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(GLSL_VERSION);

  float vertices[] = {
      -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  //
      0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  //
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 1.0f,  //
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 1.0f,  //
      -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, 1.0f,  //
      -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  //

      -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 1.0f,  //
      0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, 1.0f,  //
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  //
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  //
      -0.5f, 0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  //
      -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 1.0f,  //

      -0.5f, 0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  //
      -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, 1.0f,  //
      -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  //
      -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  //
      -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 1.0f,  //
      -0.5f, 0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  //

      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  //
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 1.0f,  //
      0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  //
      0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  //
      0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, 1.0f,  //
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  //

      -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  //
      0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  //
      0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, 1.0f,  //
      0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, 1.0f,  //
      -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 1.0f,  //
      -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  //

      -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, 1.0f,  //
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 1.0f,  //
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  //
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  //
      -0.5f, 0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  //
      -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, 1.0f,  //
  };

  uint32_t indices[] = {
      0, 1, 3,  // first triangle
      1, 2, 3   // second triangle
  };

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
  //                       (void*)(6 * sizeof(float)));
  // glEnableVertexAttribArray(2);

  shader.CreateShader(TRIANGLE_VERTEX_SHADER, TRIANGLE_FRAGMENT_SHADER);

  cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
  cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

Lucid::~Lucid() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
};

void Lucid::Update(double dt) {
  // cameraFront = input->direction;
  // std::cout << glm::to_string(input->direction) << std::endl;
  // std::cout << glm::to_string(cameraFront) << std::endl;

  const float cameraSpeed = 5.0f;
  if (IsKeyDown('w')) {
    cameraPos += static_cast<float>(cameraSpeed * dt) * cameraFront;
  }
  if (IsKeyDown('s')) {
    cameraPos -= static_cast<float>(cameraSpeed * dt) *  cameraFront;
  }
  if (IsKeyDown('a')) {
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) *
                 static_cast<float>(cameraSpeed * dt);
  }
  if (IsKeyDown('d')) {
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) *
                 static_cast<float>(cameraSpeed * dt);
  }

  glm::vec3 cubePositions[] = {glm::vec3(0.0f, 0.0f, 0.0f),      //
                               glm::vec3(2.0f, 5.0f, -15.0f),    //
                               glm::vec3(-1.5f, -2.2f, -2.5f),   //
                               glm::vec3(-3.8f, -2.0f, -12.3f),  //
                               glm::vec3(2.4f, -0.4f, -3.5f),    //
                               glm::vec3(-1.7f, 3.0f, -7.5f),    //
                               glm::vec3(1.3f, -2.0f, -2.5f),    //
                               glm::vec3(1.5f, 2.0f, -2.5f),     //
                               glm::vec3(1.5f, 0.2f, -1.5f),     //
                               glm::vec3(-1.3f, 1.0f, -1.5f)};   //

  view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

  glm::mat4 projection =
      glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

  shader.Bind();
  shader.SetUniformMatFloat4("view", view);
  shader.SetUniformMatFloat4("projection", projection);

  glBindVertexArray(VAO);

  for (size_t i = 0; i < 10; i++) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cubePositions[i]);

    float angle = 20.0f * i;

    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f),
                        glm::vec3(0.5f, 1.0f, 0.0f));

    shader.SetUniformMatFloat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }

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


void Lucid::UpdateCameraVector(float xOffset, float yOffset) {
  const float sensitivity = 0.1f;

  xOffset *= sensitivity;
  yOffset *= sensitivity;

  std::cout << "xOffset : " << xOffset << std::endl;
  std::cout << "yOffset : " << yOffset << std::endl;

  yaw += xOffset;
  pitch += yOffset;

  std::cout << "yaw : " << yaw << std::endl;
  std::cout << "pitch : " << pitch << std::endl;

  if (pitch > 89.0f) pitch = 89.0f;
  if (pitch < -89.0f) pitch = -89.0f;

  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

  cameraPos = glm::normalize(glm::cross(cameraFront, cameraUp));
  cameraUp = glm::normalize(glm::cross(cameraPos, cameraFront));
  cameraFront = glm::normalize(front);
}

