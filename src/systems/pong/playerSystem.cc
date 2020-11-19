#include "playerSystem.h"

PlayerSystem::PlayerSystem() {
}

PlayerSystem::~PlayerSystem() {
}

void PlayerSystem::Update(double dt, Registry* registry, Input* input) {
  PongRules& pongRules = registry->GetComponent<PongRules>();
  Transform* playerTransform = registry->GetComponent<Transform>(pongRules.playerPaddleID);

  const float PLAYER_MOVE_SPEED = 0.05f;

  // I think pong only moves from left to right, so this should only move the z-values
  // Note : this should not be directly translating and should be 'moved' by the
  // movement system

  // Move the player paddle
  // if (input->IsKeyDown('i')) playerTransform->position.x += PLAYER_MOVE_SPEED;
  // if (input->IsKeyDown('k')) playerTransform->position.x -= PLAYER_MOVE_SPEED;
  if (input->IsKeyDown('j')) playerTransform->position.z += PLAYER_MOVE_SPEED;
  if (input->IsKeyDown('l')) playerTransform->position.z -= PLAYER_MOVE_SPEED;
}