#include "movementSystem.h"

MovementSystem::MovementSystem() {
}

MovementSystem::~MovementSystem() {
}

void MovementSystem::Update(double dt, Registry* registry, Input* input) {
  PongRules& pongRules = registry->GetComponent<PongRules>();

  registry->GetComponentsIter<Sphere, Transform, RigidBody>()->Each(
      [dt, &pongRules, &registry](Sphere& sphere, Transform& transform, RigidBody& rigidBody) {
        const float MovementSpeed = 0.1f;

        // TODO : This should be a reference not a pointer
        Transform* playerTransform = registry->GetComponent<Transform>(pongRules.playerPaddleID);
        Transform* aiTransform = registry->GetComponent<Transform>(pongRules.aiPaddleID);

        // Move the ball in conjunction with the rigidbody velocity
        transform.position += rigidBody.velocity;
      });
}