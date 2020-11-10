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
  HandleMousePan(dt, input);
  HandleMouseScroll(dt, input);
  HandleKeyboardPan(dt, input);
  HandleKeyboardInput(dt, registry, input);

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  camera->UpdateView();

  std::vector<void*> modelComponents = registry->GetComponents<Model, Transform>();
  auto* models = static_cast<ComponentVector<Model>*>(modelComponents[0]);
  auto* transforms = static_cast<ComponentVector<Transform>*>(modelComponents[1]);

  std::vector<void*> cubeComponents = registry->GetComponents<Cube, Transform>();
  auto* cubes = static_cast<ComponentVector<Cube>*>(cubeComponents[0]);
  auto* cubeTransforms = static_cast<ComponentVector<Transform>*>(cubeComponents[1]);

  ShaderResource& shaderResource = registry->GetComponent<ShaderResource>();

  SceneRender& sceneRender = registry->GetComponent<SceneRender>();
  sceneRender.textureID = texture;

  // Show draw scaffold
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  shaderResource.modelShader.Bind();
  shaderResource.modelShader.SetUniformMatFloat4("projection", camera->projection);
  shaderResource.modelShader.SetUniformMatFloat4("view", camera->view);

  for (size_t i = 0; i < models->Size(); i++) {
    Model* m = models->At(i);
    Transform* t = transforms->At(i);

    glm::mat4 model = glm::mat4(1.0f);

    // Move the model according to transform.position
    model = glm::translate(model, t->position);
    // TODO : Multiply by transform rotation
    // Note : using a vec3 for rotation might be quaternion related rather than
    // euler angles related
    // model = glm::rotate(model, t->rotation, 30);
    model = glm::scale(model, t->scale);

    shaderResource.modelShader.SetUniformMatFloat4("model", model);
    renderer->DrawModel(*m, shaderResource.modelShader);
  }

  shaderResource.modelShader.Unbind();

  shaderResource.cubeShader.Bind();
  shaderResource.cubeShader.SetUniformMatFloat4("projection", camera->projection);
  shaderResource.cubeShader.SetUniformMatFloat4("view", camera->view);

  for (size_t i = 0; i < cubes->Size(); i++) {
    Cube* c = cubes->At(i);
    Transform* t = cubeTransforms->At(i);

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, t->position);
    model = glm::scale(model, t->scale);

    shaderResource.cubeShader.SetUniformMatFloat4("model", model);
    renderer->DrawCube(*c, shaderResource.cubeShader);
  }

  shaderResource.cubeShader.Unbind();

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

// Note : temporary
void RenderSystem::HandleKeyboardInput(double dt, Registry* registry, Input* input) {
  if (input->IsKeyDown('1')) {
    Entity cubeID = registry->GetAvailableEntityId();

    registry->CreateEntity<Cube, Transform>(cubeID);

    Transform* transform = registry->GetComponent<Transform>(cubeID);
    transform->position = camera->cameraPos;
    transform->scale = glm::vec3(1.0f);

    input->SetKeyOff('1');
  }
}