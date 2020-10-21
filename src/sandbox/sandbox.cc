#include "sandbox.h"

Sandbox::Sandbox(Registry* registry, Input* input, GLFWwindow* window) {
  Sandbox::registry = registry;
  Sandbox::input = input;
  Sandbox::window = window;

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable Docking
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

Sandbox::~Sandbox() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
};

void Sandbox::Update(double dt) {
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

  InitializeGUI();
}

void Sandbox::InitializeGUI() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGuiIO& io = ImGui::GetIO();
  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
  {
    ImGui::DockSpaceOverViewport();
    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("File")) {
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Edit")) {
        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }
  }
  ImGui::ShowDemoWindow();

  ImGui::Begin("Sandbox");
  ImGui::BeginTabBar("Sandbox Tab Bar");

  //  ImGui::BeginTabItem("Hello");
  //  ImGui::Text("something");
  //  ImGui::EndTabItem();

  ImGui::EndTabBar();
  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}