#include "fpsSystem.h"

FpsSystem::FpsSystem()
{
}

FpsSystem::~FpsSystem()
{
}

void FpsSystem::Update(float dt, Registry* registry, Input* input)
{
  Camera* camera = GetActiveCameraPtr(registry);
  Transform* transform = GetActiveTransformPtr(registry);

  const float SPEED = 5.0f;

  if (input->IsKeyDown('W')) Translate(transform, glm::vec3(0.0f, 0.0f, SPEED * dt));
  if (input->IsKeyDown('S')) Translate(transform, glm::vec3(0.0f, 0.0f, -(SPEED * dt)));
  if (input->IsKeyDown('A')) Translate(transform, glm::vec3(SPEED * dt, 0.0f, 0.0f));
  if (input->IsKeyDown('D')) Translate(transform, glm::vec3(-(SPEED * dt), 0.0f, 0.0f));

  if (input->lastX != input->GetMouseX() || input->lastY != input->GetMouseY())
  {
    float offsetX = input->GetMouseX() - input->lastX;
    float offsetY = input->GetMouseY() - input->lastY;

    input->lastX = input->GetMouseX();
    input->lastY = input->GetMouseY();

    PanCamera(dt, camera, transform, offsetX, offsetY);
  }
}