#include <vector>
#include "ecs.h"
#include "component.h"
#include "gtest/gtest.h"

#include "renderSystem.h"
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

// The ray & box intersection current lies in the renderSystem, so the test will be
// using renderSystem to do the testing, but in the future I think there should
// be a centralised place that hosts the collision checks between the primitives
TEST(PhysicsAndRenderCollision, RayBoxColliding) {
  Registry* registry = new Registry();
  RenderSystem* renderSystem = new RenderSystem();

  registry->RegisterArchetype<Cube, Transform>();
  Entity first = registry->GetAvailableEntityId();
  registry->CreateEntity<Cube, Transform>(first);

  Transform* transform = registry->GetComponent<Transform>(first);
  transform->position = {0.0f, 0.0f, 0.0f};

  // Origin faces towards the origin of the world but a little further back?
  auto origin = glm::vec3(0.0f, 0.0f, 10.0f);
  // Ray goes towards the z-axis.
  auto ray = glm::vec3(0.0f, 0.0f, -1.0f);

  // renderSystem->RayBoundingBoxCollisionCheck(origin, ray, );
}

TEST(PhysicsAndRenderCollision, RayBoxNotColliding) {
}