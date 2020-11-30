#include "aiSystem.h"

AiSystem::AiSystem() {
}

AiSystem::~AiSystem() {
}

void AiSystem::Update(double dt, Registry* registry, Input* input) {
  PongRules& pongRules = registry->GetComponent<PongRules>();

  Transform* aiTransform = registry->GetComponent<Transform>(pongRules.aiPaddleID);
  Transform* ballTransform = registry->GetComponent<Transform>(pongRules.ballID);
  RigidBody* aiRigidBody = registry->GetComponent<RigidBody>(pongRules.aiPaddleID);

  // Just move the ai paddle in motion with the ball transform
  if (ballTransform->position.z > aiTransform->position.z) {
    aiRigidBody->velocity = glm::vec3(0.0f, 0.0f, PADDLE_MOVE_SPEED);
  } else if (ballTransform->position.z < aiTransform->position.z) {
    aiRigidBody->velocity = glm::vec3(0.0f, 0.0f, -PADDLE_MOVE_SPEED);
  }
}