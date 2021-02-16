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
  FpsRules& fpsRules = registry->GetResource<FpsRules>();

  const float SPEED = 20.0f;
  const float VELOCITY = 1.0f;

  registry->GetComponent<Font>(fpsRules.fontId).text = "Score : " + std::to_string(fpsRules.score);

  // Move player around
  if (input->IsKeyDown('W')) Translate(transform, glm::vec3(0.0f, 0.0f, SPEED * dt));
  if (input->IsKeyDown('S')) Translate(transform, glm::vec3(0.0f, 0.0f, -(SPEED * dt)));
  if (input->IsKeyDown('A')) Translate(transform, glm::vec3(SPEED * dt, 0.0f, 0.0f));
  if (input->IsKeyDown('D')) Translate(transform, glm::vec3(-(SPEED * dt), 0.0f, 0.0f));

  // Always reset it to 1.0f
  // transform->position.y = 1.0f;

  // Move mouse around the cursor
  if (input->lastX != input->GetMouseX() || input->lastY != input->GetMouseY())
  {
    float offsetX = input->GetMouseX() - input->lastX;
    float offsetY = input->GetMouseY() - input->lastY;

    input->lastX = input->GetMouseX();
    input->lastY = input->GetMouseY();

    PanCamera(dt, camera, transform, offsetX, offsetY);
  }

  // if (input->IsMouseLDown())
  if (input->IsKeyDown(GLFW_KEY_SPACE))
  {
    input->SetKeyOff(GLFW_KEY_SPACE);
    glm::vec3 rayDirection = GetRayDirection(registry, input, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    // std::cout << glm::to_string(rayDirection) << std::endl;

    Entity bulletId = registry->GetAvailableEntityId();
    registry->CreateEntity<Cube, Transform, RigidBody, ColliderCube>(bulletId);

    // Set correct component values
    registry->GetComponent<Transform>(bulletId).position = GetPositionInWorld(transform);
    registry->GetComponent<RigidBody>(bulletId).velocity = rayDirection;
    registry->GetComponent<RigidBody>(bulletId).applyGravity = false;
    registry->GetComponent<Sound>(fpsRules.soundId).play = true;
  }

  std::vector<Entity> toDelete;
  registry->GetComponentsIter<Model, Transform, RigidBody, Enemy>()->EachWithID(
      [&](Entity id, Model& model, Transform& enemyTransform, RigidBody& rigidBody, Enemy& enemy) {
        float length = glm::length(-transform->position - enemyTransform.position);
        // if (colliderCube.collided)
        // {
        //   fpsRules.score += 1;
        //   toDelete.push_back(id);
        // }
        if (length < 10)
        {
          fpsRules.score += 1;
          toDelete.push_back(id);
        }
        else
        {
          rigidBody.velocity =
              glm::normalize(transform->position - enemyTransform.position) * 0.10f;
        }
      });

  for (size_t i = 0; i < toDelete.size(); i++)
  {
    registry->DeleteEntity<Deleter>(toDelete[i]);
  }

  std::vector<Entity> bulletDelete;
  registry->GetComponentsIter<Cube, Transform, RigidBody, ColliderCube>()->EachWithID(
      [&](Entity id, Cube& sphere, Transform& bulletTransform, RigidBody& rigidBody,
          ColliderCube& colliderCube) {
        if (glm::length(-transform->position - bulletTransform.position) > 100)
        {
          bulletDelete.push_back(id);
        }
      });

  for (size_t i = 0; i < bulletDelete.size(); i++)
  {
    registry->DeleteEntity<Deleter>(bulletDelete[i]);
  }

  // FpsRules fpsRules = registry->GetResource<FpsRules>();
  // if (fpsRules.enemyCount < fpsRules.maxEnemyCount)
  // {
  // }
}