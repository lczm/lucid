#include "renderSystem.h"

RenderSystem::RenderSystem() {
  RenderSystem::renderer = new Renderer();
  RenderSystem::camera = new Camera();
}

RenderSystem::~RenderSystem() = default;

void RenderSystem::Update(double dt, Registry* registry, Input* input) {
  HandleMousePan(dt, input);
  HandleKeyboardPan(dt, input);

  camera->UpdateView();

  std::vector<void*> modelComponents = registry->GetComponents<Model>();
  std::vector<void*> shaderComponents = registry->GetComponents<Shader>();

  auto* models = static_cast<ComponentVector<Model>*>(modelComponents[0]);
  auto* shaders = static_cast<ComponentVector<Shader>*>(shaderComponents[0]);

  Shader* shader = shaders->At(0);

  shader->Bind();
  shader->SetUniformMatFloat4("projection", camera->GetProjection());
  shader->SetUniformMatFloat4("view", camera->GetView());

  for (size_t i = 0; i < models->Size(); i++) {
    Model* m = models->At(i);
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(1.0f, 1.0f, 1.0f * (i * 5)));
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
  } else if (input->IsMouseRDown() &&
             (input->lastX != input->GetMouseX() || input->lastY != input->GetMouseY())) {
    float offsetX = input->GetMouseX() - input->lastX;
    float offsetY = input->GetMouseY() - input->lastY;

    input->lastX = input->GetMouseX();
    input->lastY = input->GetMouseY();

    camera->UpdateCameraVector(offsetX, offsetY);
  }

  // Scroll up
  if (input->scroll == 1) {
    camera->cameraPos += static_cast<float>(SCROLL_SPEED * dt) * camera->cameraFront;
  }

  // Scroll down
  if (input->scroll == -1) {
    camera->cameraPos -= static_cast<float>(SCROLL_SPEED * dt) * camera->cameraFront;
  }

  // Reset the scroll variable once done
  input->scroll = 0;
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