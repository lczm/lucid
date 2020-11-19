#include "movementSystem.h"

MovementSystem::MovementSystem() {
}

MovementSystem::~MovementSystem() {
}

void MovementSystem::Update(double dt, Registry* registry, Input* input) {
  PongRules& pongRules = registry->GetComponent<PongRules>();

  // Get all entities with transforms and rigidbodys and move them
  registry->GetComponentsIter<Transform, RigidBody>()->Each(
      [dt](Transform& transform, RigidBody& rigidBody) {
        transform.position += rigidBody.velocity;
      });
}