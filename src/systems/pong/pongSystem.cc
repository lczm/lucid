#include "pongSystem.h"

PongSystem::PongSystem() {
}

PongSystem::~PongSystem() {
}

void PongSystem::Update(double dt, Registry* registry, Input* input) {
  registry->GetComponentsIter<Sphere, RigidBody, BoundingBoxCube>()->Each(
      [dt](Sphere& sphere, RigidBody& rigidBody, BoundingBoxCube& boundingBoxCube) {
        if (boundingBoxCube.collided) {
          rigidBody.velocity = glm::vec3(-rigidBody.velocity.x,  //
                                         -rigidBody.velocity.y,  //
                                         -rigidBody.velocity.z);
        }
      });
}