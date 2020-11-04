#include <vector>
#include "ecs.h"
#include "component.h"
#include "gtest/gtest.h"

struct TestAddStruct { int a; };

TEST(ECS, AddComponent) {
  Registry* registry = new Registry();

  // Create some archetypes
  registry->RegisterArchetype<Transform>();

  // Create some entities
  Entity entity1 = registry->GetAvailableEntityId();
  registry->CreateEntity<Transform>(entity1);

  // Add a component to entity1
  registry->AddComponent<TestAddStruct>(entity1);
  registry->FlushEntity<Transform, TestAddStruct>(entity1);

  std::vector<void*> components = registry->GetComponents<Transform, TestAddStruct>();

  auto* transformCV = static_cast<ComponentVector<Transform>*>(components[0]);
  auto* testAddStructCV = static_cast<ComponentVector<TestAddStruct>*>(components[1]);

  EXPECT_EQ(transformCV->Size(), 1);
  EXPECT_EQ(testAddStructCV->Size(), 1);
}

TEST(ECS, RemoveComponent) {
}