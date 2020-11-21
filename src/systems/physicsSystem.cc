#include "physicsSystem.h"

PhysicsSystem::PhysicsSystem() {
}

PhysicsSystem::~PhysicsSystem() {
}

void PhysicsSystem::Update(double dt, Registry* registry, Input* input) {
  // Debug Start
  std::vector<void*> components = registry->GetComponents<Transform, RigidBody, BoundingBoxCube>();

  auto* transformComponents = static_cast<ComponentVector<Transform>*>(components[0]);
  auto* rigidBodyComponents = static_cast<ComponentVector<RigidBody>*>(components[1]);
  auto* boundingBoxCubeComponents = static_cast<ComponentVector<BoundingBoxCube>*>(components[2]);

  // If there are less than two components, can ignore checking for collisions
  if (boundingBoxCubeComponents->Size() < 2) {
    return;
  }

  // Debug :: hard-coded for sphere collisions
  for (size_t i = 0; i < boundingBoxCubeComponents->Size(); i++) {
    for (size_t j = 0; j < boundingBoxCubeComponents->Size(); j++) {
      // TODO can cache results,
      // i.e. if A collides with B, there is no need to check if
      // B collides with A
      if (i == j) continue;

      BoundingBoxCube& collider1 = boundingBoxCubeComponents->At(i);
      Transform& transform1 = transformComponents->At(i);

      BoundingBoxCube& collider2 = boundingBoxCubeComponents->At(j);
      Transform& transform2 = transformComponents->At(j);

      CheckCollision(collider1, transform1, collider2, transform2);
    }
  }
  // Debug End
}

void PhysicsSystem::CheckCollision(BoundingBoxCube& boundingBoxCube, Transform& transform,
                                   BoundingBoxCube& boundingBoxCubeOther,
                                   Transform& transformOther) {
  // Assume that it is a box/cube...

  // Check for x-axis collision
  // Check for y-axis collision

  // return (a.minX <= b.maxX && a.maxX >= b.minX) &&
  //        (a.minY <= b.maxY && a.maxY >= b.minY) &&
  //        (a.minZ <= b.maxZ && a.maxZ >= b.minZ);
}

void PhysicsSystem::GetAxisAlignedBoundingBox(ColliderSphere& collider, Transform& transform) {
}

void PhysicsSystem::GetOrientedBoundingBox(ColliderSphere& collider, Transform& transform) {
}
