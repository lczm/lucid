#include "playerSystem.h"

PlayerSystem::PlayerSystem()
{
}

PlayerSystem::~PlayerSystem()
{
}

void PlayerSystem::Update(float dt, Registry* registry, Input* input)
{
  PongRules& pongRules = registry->GetComponent<PongRules>();
  RigidBody* playerRigidBody = registry->GetComponent<RigidBody>(pongRules.playerPaddleID);

  // I think pong only moves from left to right, so this should only move the z-values
  // Note : this should not be directly translating and should be 'moved' by the
  // movement system

  // Move the player paddle
  // if (input->IsKeyDown('I')) playerTransform->position.x += PLAYER_MOVE_SPEED;
  // if (input->IsKeyDown('K')) playerTransform->position.x -= PLAYER_MOVE_SPEED;
  if (input->IsKeyDown('J')) playerRigidBody->velocity = glm::vec3(0.0f, 0.0f, PADDLE_MOVE_SPEED);
  if (input->IsKeyDown('L')) playerRigidBody->velocity = glm::vec3(0.0f, 0.0f, -PADDLE_MOVE_SPEED);
}