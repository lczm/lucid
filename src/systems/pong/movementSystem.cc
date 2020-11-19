#include "movementSystem.h"

MovementSystem::MovementSystem() {
}

MovementSystem::~MovementSystem() {
}

void MovementSystem::Update(double dt, Registry* registry, Input* input) {
  PongRules& pongRules = registry->GetComponent<PongRules>();

  // registry->GetComponentsIter<Sphere, Transform, RigidBody>()->Each(
  //     [dt, &pongRules](Sphere& sphere, Transform& transform, RigidBody& rigidBody) {
  //       // If it is the player's turn, the ball will be moving towards the player
  //       if (pongRules.turn == Turn::PLAYER) {
  //       } else if (pongRules.turn == Turn::AI) {
  //       }
  //     });
}