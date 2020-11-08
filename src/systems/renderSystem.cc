#include "renderSystem.h"

RenderSystem::RenderSystem() {
  RenderSystem::renderer = new Renderer();
  RenderSystem::camera = new Camera();

  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  // TODO : The SCREEN_WIDTH and SCREEN_HEIGHT here has to be changed
  // whenever the game is resized
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE,
               NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);

  // TODO : The SCREEN_WIDTH and SCREEN_HEIGHT here has to be changed
  // whenever the game is resized
  glRenderbufferStorage(
      GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1440,
      900);  // use a single renderbuffer object for both a depth AND stencil buffer.
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                            rbo);  // now actually attach it

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RenderSystem::~RenderSystem() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteFramebuffers(1, &fbo);
}

void RenderSystem::Update(double dt, Registry* registry, Input* input) {
  // WindowType windowType = input->activeWindow;
  // if (windowType == WindowType::None) {
  //   std::cout << "none" << std::endl;
  // } else if (windowType == WindowType::Scene) {
  //   std::cout << "scene" << std::endl;
  // } else if (windowType == WindowType::Hierarchy) {
  //   std::cout << "hierarchy" << std::endl;
  // } else if (windowType == WindowType::Project) {
  //   std::cout << "project" << std::endl;
  // } else if (windowType == WindowType::Console) {
  //   std::cout << "console" << std::endl;
  // }

  HandleMousePan(dt, input);
  HandleMouseScroll(dt, input);
  HandleKeyboardPan(dt, input);

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  camera->UpdateView();

  std::vector<void*> modelComponents = registry->GetComponents<Model>();
  std::vector<void*> shaderComponents = registry->GetComponents<Shader>();

  auto* models = static_cast<ComponentVector<Model>*>(modelComponents[0]);
  auto* shaders = static_cast<ComponentVector<Shader>*>(shaderComponents[0]);

  Shader* shader = shaders->At(0);

  shader->Bind();
  shader->SetUniformMatFloat4("projection", camera->projection);
  shader->SetUniformMatFloat4("view", camera->view);

  for (size_t i = 0; i < models->Size(); i++) {
    Model* m = models->At(i);
    glm::mat4 model = glm::mat4(1.0f);

    // Move the model according to transform.position
    model = glm::translate(model, glm::vec3(1.0f, 1.0f, 1.0f * (i * 5)));

    // TODO : Scale the model according to transform.scale

    // Avocado model is very small, scale it all the way up (debugging purposes)
    if (m->GetTag() == "avocado") {
      model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));
    } else {
      model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    }

    shader->SetUniformMatFloat4("model", model);
    renderer->DrawModel(*m, *shader);
  }

  shader->Unbind();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderSystem::HandleMousePan(double dt, Input* input) {
  if (input->IsMouseLDown() &&
      (input->lastX != input->GetMouseX() || input->lastY != input->GetMouseY())) {
    float offsetX = input->GetMouseX() - input->lastX;
    float offsetY = input->GetMouseY() - input->lastY;

    // Handle the x-axis movement
    camera->cameraPos -= glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp)) *
                         static_cast<float>(offsetX * dt);

    // Handle the y-axis movement
    camera->cameraPos -= glm::normalize(camera->cameraUp) * static_cast<float>(offsetY * dt);

    input->lastX = input->GetMouseX();
    input->lastY = input->GetMouseY();
    return;
  }

  if (input->IsMouseRDown() &&
      (input->lastX != input->GetMouseX() || input->lastY != input->GetMouseY())) {
    float offsetX = input->GetMouseX() - input->lastX;
    float offsetY = input->GetMouseY() - input->lastY;

    input->lastX = input->GetMouseX();
    input->lastY = input->GetMouseY();

    camera->UpdateCameraVector(offsetX, offsetY);
    return;
  }
}

void RenderSystem::HandleMouseScroll(double dt, Input* input) {
  // Scroll up
  if (input->GetScrollState() == 1) {
    camera->cameraPos += static_cast<float>(SCROLL_SPEED * dt) * camera->cameraFront;
  }

  // Scroll down
  if (input->GetScrollState() == -1) {
    camera->cameraPos -= static_cast<float>(SCROLL_SPEED * dt) * camera->cameraFront;
  }

  // Reset the scroll variable once done
  input->scroll = 0;
  return;
}

void RenderSystem::HandleKeyboardPan(double dt, Input* input) {
  if (input->IsKeyDown('w')) {
    camera->cameraPos += static_cast<float>(CAMERA_SPEED * dt) * camera->cameraFront;
  }

  if (input->IsKeyDown('s')) {
    camera->cameraPos -= static_cast<float>(CAMERA_SPEED * dt) * camera->cameraFront;
  }

  if (input->IsKeyDown('a')) {
    camera->cameraPos -= glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp)) *
                         static_cast<float>(CAMERA_SPEED * dt);
  }

  if (input->IsKeyDown('d')) {
    camera->cameraPos += glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp)) *
                         static_cast<float>(CAMERA_SPEED * dt);
  }
}