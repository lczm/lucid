#include <vector>
#include <iostream>

// #include "lucid.h"
// #include "component.h"
// #include "gtest/gtest.h"
// #include "constants.h"
// #include "errors.h"
// #include "ecs.h"
// #include "input.h"
// #include "utils.h"
// #include "engineStartup.h"

// #include "renderSystem.h"
// #include "physicsSystem.h"
// #include "test-utils.h"

/*
TEST_F(TestsGL, BoxBoxColliding)
{
  Registry* registry = new Registry();
  InitArchetypes(registry);
  InitEngineComponents(registry);

  Input* input = new Input();

  registry->RegisterSystem(new PhysicsSystem());

  // Register systems and archetypes
  registry->RegisterArchetype<Cube, Transform, RigidBody, ColliderCube>();

  // Get the entities
  Entity first = registry->GetAvailableEntityId();
  Entity second = registry->GetAvailableEntityId();

  // Create the entities
  registry->CreateEntity<Cube, Transform, RigidBody, ColliderCube>(first);
  registry->CreateEntity<Cube, Transform, RigidBody, ColliderCube>(second);

  Transform& firstTransform = registry->GetComponent<Transform>(first);
  Transform& secondTransform = registry->GetComponent<Transform>(second);

  firstTransform.position = {0.0f, 0.0f, 0.0f};
  secondTransform.position = {1.0f, 0.0f, 0.0f};

  // Arbitrary value, does not really matter
  const double dt = 0.005f;

  // Update the physics system
  registry->UpdateSystems(dt, input);

  // Check that both BoundingBoxCube(s) are colliding
  registry->GetComponentsIter<ColliderCube>()->Each(
      [](ColliderCube& colliderCube) { ASSERT_TRUE(colliderCube.collided); });
}

// Test that it detects that it is not colliding
TEST_F(TestsGL, BoxBoxNotColliding)
{
  Registry* registry = new Registry();
  InitArchetypes(registry);
  InitEngineComponents(registry);

  Input* input = new Input();

  registry->RegisterSystem(new PhysicsSystem());

  // Register systems and archetypes
  registry->RegisterArchetype<Cube, Transform, RigidBody, ColliderCube>();

  // Get the entities
  Entity first = registry->GetAvailableEntityId();
  Entity second = registry->GetAvailableEntityId();

  // Create the entities
  registry->CreateEntity<Cube, Transform, RigidBody, ColliderCube>(first);
  registry->CreateEntity<Cube, Transform, RigidBody, ColliderCube>(second);

  Transform& firstTransform = registry->GetComponent<Transform>(first);
  Transform& secondTransform = registry->GetComponent<Transform>(second);

  firstTransform.position = {0.0f, 0.0f, 0.0f};
  secondTransform.position = {10.0f, 0.0f, 0.0f};

  // Arbitrary value, does not really matter
  const double dt = 0.005f;

  // Update the physics system
  registry->UpdateSystems(dt, input);

  // Check that both BoundingBoxCube(s) are colliding
  registry->GetComponentsIter<ColliderCube>()->Each(
      [](ColliderCube& colliderCube) { ASSERT_FALSE(colliderCube.collided); });
}

// The ray & box intersection current lies in the renderSystem, so the test will be
// using renderSystem to do the testing, but in the future I think there should
// be a centralised place that hosts the collision checks between the primitives
TEST_F(TestsGL, RayBoxColliding)
{
  Registry* registry = new Registry();
  InitArchetypes(registry);
  InitEngineComponents(registry);

  RenderSystem* renderSystem = new RenderSystem(registry);

  Entity first = registry->GetAvailableEntityId();
  registry->CreateEntity<Cube, Transform>(first);

  Transform& transform = registry->GetComponent<Transform>(first);
  transform.position = {0.0f, 0.0f, 0.0f};

  // Origin faces towards the origin of the world but a little further back?
  auto origin = glm::vec3(0.0f, 0.0f, 10.0f);
  // Ray goes towards the z-axis.
  auto ray = glm::vec3(0.0f, 0.0f, -1.0f);

  glm::mat4 matrixModel = glm::mat4(1.0f);
  matrixModel = glm::translate(matrixModel, transform.position);
  matrixModel = glm::scale(matrixModel, transform.scale);

  std::vector<glm::vec4> vertices;
  vertices.reserve(boundingBoxCubeVertices.size() / 3);

  for (size_t i = 0; i < boundingBoxCubeVertices.size(); i += 3)
  {
    vertices.push_back(matrixModel * glm::vec4(boundingBoxCubeVertices[i],
                                               boundingBoxCubeVertices[i + 1],
                                               boundingBoxCubeVertices[i + 2], 1.0f));
  }

  BoundingBox bb = renderSystem->GetBoundingBox(vertices);
  auto collidedAndLength = renderSystem->RayBoundingBoxCollisionCheck(origin, ray, bb);

  ASSERT_TRUE(std::get<bool>(collidedAndLength));
}

TEST_F(TestsGL, RayBoxNotColliding)
{
  Registry* registry = new Registry();
  InitArchetypes(registry);
  InitEngineComponents(registry);

  RenderSystem* renderSystem = new RenderSystem(registry);

  registry->RegisterArchetype<Cube, Transform>();
  Entity first = registry->GetAvailableEntityId();
  registry->CreateEntity<Cube, Transform>(first);

  Transform& transform = registry->GetComponent<Transform>(first);
  transform.position = {0.0f, 0.0f, 0.0f};

  // Origin faces towards the origin of the world but a little further back?
  auto origin = glm::vec3(0.0f, 0.0f, 10.0f);
  // Ray goes away the z-axis. (Unlike the above test, this will test for non colliding)
  auto ray = glm::vec3(0.0f, 0.0f, 1.0f);

  glm::mat4 matrixModel = glm::mat4(1.0f);
  matrixModel = glm::translate(matrixModel, transform.position);
  matrixModel = glm::scale(matrixModel, transform.scale);

  std::vector<glm::vec4> vertices;
  vertices.reserve(boundingBoxCubeVertices.size() / 3);

  for (size_t i = 0; i < boundingBoxCubeVertices.size(); i += 3)
  {
    vertices.push_back(matrixModel * glm::vec4(boundingBoxCubeVertices[i],
                                               boundingBoxCubeVertices[i + 1],
                                               boundingBoxCubeVertices[i + 2], 1.0f));
  }

  BoundingBox bb = renderSystem->GetBoundingBox(vertices);
  auto collidedAndLength = renderSystem->RayBoundingBoxCollisionCheck(origin, ray, bb);

  ASSERT_FALSE(std::get<bool>(collidedAndLength));
}
*/