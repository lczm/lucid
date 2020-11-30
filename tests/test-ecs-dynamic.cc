#include <vector>
#include "ecs.h"
#include "component.h"
#include "gtest/gtest.h"

struct TestAddStructDynamic1 {
  int a;
};

struct TestAddStructDynamic2 {
  float b;
};

TEST(ECS, AddComponent) {
  Registry* registry = new Registry();

  // Create some archetypes
  registry->RegisterArchetype<Transform>();

  // Create some entities
  Entity entity1 = registry->GetAvailableEntityId();
  registry->CreateEntity<Transform>(entity1);

  // Add a component to entity1
  registry->AddComponent<TestAddStructDynamic1>(entity1);
  registry->FlushEntity<Transform, TestAddStructDynamic1>(entity1);

  std::vector<void*> components = registry->GetComponents<Transform, TestAddStructDynamic1>();

  auto* transformCV = static_cast<ComponentVector<Transform>*>(components[0]);
  auto* testAddStructCV = static_cast<ComponentVector<TestAddStructDynamic1>*>(components[1]);

  EXPECT_EQ(transformCV->Size(), 1);
  EXPECT_EQ(testAddStructCV->Size(), 1);

  // Add another component to entity1
  // registry->AddComponent<TestAddStructDynamic2>(entity1);
  // registry->FlushEntity<Transform, TestAddStructDynamic1, TestAddStructDynamic2>(entity1);

  // components = registry->GetComponents<Transform, TestAddStructDynamic1,
  // TestAddStructDynamic2>();

  // transformCV = static_cast<ComponentVector<Transform>*>(components[0]);
  // testAddStructCV = static_cast<ComponentVector<TestAddStructDynamic1>*>(components[1]);
  // auto* testAddStruct2CV = static_cast<ComponentVector<TestAddStructDynamic2>*>(components[2]);

  // EXPECT_EQ(transformCV->Size(), 1);
  // EXPECT_EQ(testAddStructCV->Size(), 1);
  // EXPECT_EQ(testAddStruct2CV->Size(), 1);
}

TEST(ECS, RemoveComponent) {
}

TEST(ECS, AddRemoveComponent) {
}