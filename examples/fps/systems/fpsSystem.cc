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
  FpsRules fpsRules = registry->GetResource<FpsRules>();

  const float SPEED = 20.0f;
  const float VELOCITY = 1.0f;

  registry->GetComponent<Font>(fpsRules.fontId).text = "Score : " + std::to_string(fpsRules.score);

  // Move player around
  if (input->IsKeyDown('W')) Translate(transform, glm::vec3(0.0f, 0.0f, SPEED * dt));
  if (input->IsKeyDown('S')) Translate(transform, glm::vec3(0.0f, 0.0f, -(SPEED * dt)));
  if (input->IsKeyDown('A')) Translate(transform, glm::vec3(SPEED * dt, 0.0f, 0.0f));
  if (input->IsKeyDown('D')) Translate(transform, glm::vec3(-(SPEED * dt), 0.0f, 0.0f));

  // Move mouse around the cursor
  if (input->lastX != input->GetMouseX() || input->lastY != input->GetMouseY())
  {
    float offsetX = input->GetMouseX() - input->lastX;
    float offsetY = input->GetMouseY() - input->lastY;

    input->lastX = input->GetMouseX();
    input->lastY = input->GetMouseY();

    PanCamera(dt, camera, transform, offsetX, offsetY);
  }

  if (input->IsMouseLDown())
  {
    glm::vec3 rayDirection = GetRayDirection(registry, input, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    // std::cout << glm::to_string(rayDirection) << std::endl;

    Entity bulletId = registry->GetAvailableEntityId();
    registry->CreateEntity<Sphere, Transform, RigidBody>(bulletId);

    // Set correct component values
    registry->GetComponent<Transform>(bulletId).position = GetPositionInWorld(transform);
    registry->GetComponent<RigidBody>(bulletId).velocity = rayDirection;
    registry->GetComponent<RigidBody>(bulletId).applyGravity = false;
    registry->GetComponent<Sound>(fpsRules.soundId).play = true;
  }

  // FpsRules fpsRules = registry->GetResource<FpsRules>();
  // if (fpsRules.enemyCount < fpsRules.maxEnemyCount)
  // {
  // }
}