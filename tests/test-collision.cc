#include <vector>
#include "ecs.h"
#include "component.h"
#include "gtest/gtest.h"

#include "physicsSystem.h"

TEST(PhysicsCollision, BoxBoxColliding) {
  Registry* registry = new Registry();
  Input* input = new Input();

  registry->RegisterSystem(new PhysicsSystem());

  // Register systems and archetypes
  registry->RegisterArchetype<Cube, Transform, RigidBody, BoundingBoxCube>();

  // Get the entities
  Entity first = registry->GetAvailableEntityId();
  Entity second = registry->GetAvailableEntityId();

  // Create the entities
  registry->CreateEntity<Cube, Transform, RigidBody, BoundingBoxCube>(first);
  registry->CreateEntity<Cube, Transform, RigidBody, BoundingBoxCube>(second);

  Transform* firstTransform = registry->GetComponent<Transform>(first);
  Transform* secondTransform = registry->GetComponent<Transform>(second);

  firstTransform->position = {0.0f, 0.0f, 0.0f};
  secondTransform->position = {1.0f, 0.0f, 0.0f};

  // Arbitrary value, does not really matter
  const double dt = 0.005f;

  // Update the physics system
  registry->UpdateSystems(dt, input);

  // Check that both BoundingBoxCube(s) are colliding
  registry->GetComponentsIter<BoundingBoxCube>()->Each(
      [](BoundingBoxCube& boundingBoxCube) { EXPECT_TRUE(boundingBoxCube.collided); });
}

// Test that it detects that it is not colliding
TEST(PhysicsCollision, BoxBoxNotColliding) {
  Registry* registry = new Registry();
  Input* input = new Input();

  registry->RegisterSystem(new PhysicsSystem());

  // Register systems and archetypes
  registry->RegisterArchetype<Cube, Transform, RigidBody, BoundingBoxCube>();

  // Get the entities
  Entity first = registry->GetAvailableEntityId();
  Entity second = registry->GetAvailableEntityId();

  // Create the entities
  registry->CreateEntity<Cube, Transform, RigidBody, BoundingBoxCube>(first);
  registry->CreateEntity<Cube, Transform, RigidBody, BoundingBoxCube>(second);

  Transform* firstTransform = registry->GetComponent<Transform>(first);
  Transform* secondTransform = registry->GetComponent<Transform>(second);

  firstTransform->position = {0.0f, 0.0f, 0.0f};
  secondTransform->position = {10.0f, 0.0f, 0.0f};

  // Arbitrary value, does not really matter
  const double dt = 0.005f;

  // Update the physics system
  registry->UpdateSystems(dt, input);

  // Check that both BoundingBoxCube(s) are colliding
  registry->GetComponentsIter<BoundingBoxCube>()->Each(
      [](BoundingBoxCube& boundingBoxCube) { EXPECT_FALSE(boundingBoxCube.collided); });
}
