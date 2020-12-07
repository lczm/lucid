#include "pongSystem.h"

PongSystem::PongSystem() {
}

PongSystem::~PongSystem() {
}

void PongSystem::Update(double dt, Registry* registry, Input* input) {
  registry->GetComponentsIter<Sphere, RigidBody, ColliderCube>()->Each(
      [&](Sphere& sphere, RigidBody& rigidBody, ColliderCube& colliderCube) {
        if (colliderCube.collided) {
          rigidBody.velocity = glm::vec3(-rigidBody.velocity.x,  //
                                         -rigidBody.velocity.y,  //
                                         -rigidBody.velocity.z);
        }
      });
}