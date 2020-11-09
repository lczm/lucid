// This is needed becuase component imports texture which uses stbi etc...
#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION

#include <vector>
#include "ecs.h"
#include "component.h"
#include "gtest/gtest.h"

/*
  This is to test that the ComponentVector<T> grows for whatever type whenever a
  new entity is added.
 */

struct TestAddStruct1 {
  int a = 0;
  int b = 0;
};

struct TestAddStruct2 {
  int c = 0;
  int d = 0;
};

TEST(ECS, ComponentVectorSize) {
  Registry* registry = new Registry();

  // Create some archetypes
  registry->RegisterArchetype<TestAddStruct1>();
  registry->RegisterArchetype<TestAddStruct1, TestAddStruct2>();

  // Get some entities
  Entity entity1 = registry->GetAvailableEntityId();
  Entity entity2 = registry->GetAvailableEntityId();

  // Create the entities
  registry->CreateEntity<TestAddStruct1>(entity1);
  registry->CreateEntity<TestAddStruct1, TestAddStruct2>(entity2);

  // Test transform component size
  std::vector<void*> transformComponents = registry->GetComponents<TestAddStruct1>();
  ComponentVector<TestAddStruct1>* transformComponentVector =
      static_cast<ComponentVector<TestAddStruct1>*>(transformComponents[0]);
  EXPECT_EQ(transformComponentVector->Size(), 2);

  // Test image component size
  std::vector<void*> imageComponents = registry->GetComponents<TestAddStruct2>();
  ComponentVector<TestAddStruct2>* imageComponentVector =
      static_cast<ComponentVector<TestAddStruct2>*>(imageComponents[0]);
  EXPECT_EQ(imageComponentVector->Size(), 1);

  // Create another entity
  Entity entity3 = registry->GetAvailableEntityId();
  registry->CreateEntity<TestAddStruct1>(entity3);

  // Check that the size of the component vector for TestAddStruct1 is 3 now.
  transformComponents = registry->GetComponents<TestAddStruct1>();
  transformComponentVector = static_cast<ComponentVector<TestAddStruct1>*>(transformComponents[0]);
  EXPECT_EQ(transformComponentVector->Size(), 3);
}

/*
   Test the iteration of the componentVector
 */
TEST(ECS, ComponentVectorIteration) {
  Registry* registry = new Registry();

  // Create some archetypes
  registry->RegisterArchetype<TestAddStruct1>();
  registry->RegisterArchetype<TestAddStruct1, TestAddStruct2>();

  // Get some entities
  Entity entity1 = registry->GetAvailableEntityId();
  Entity entity2 = registry->GetAvailableEntityId();

  // Create the entities
  registry->CreateEntity<TestAddStruct1>(entity1);
  registry->CreateEntity<TestAddStruct1, TestAddStruct2>(entity2);

  TestAddStruct1 updatedComponent;
  updatedComponent.a = 500;
  updatedComponent.b = 500;

  registry->AddComponentData<TestAddStruct1>(entity1, updatedComponent);

  // Test component vector size
  std::vector<void*> transformComponents = registry->GetComponents<TestAddStruct1>();
  ComponentVector<TestAddStruct1>* transformComponentVector =
      static_cast<ComponentVector<TestAddStruct1>*>(transformComponents[0]);
  EXPECT_EQ(transformComponentVector->Size(), 2);

  // Test component vector iteration
  for (size_t i = 0; i < transformComponentVector->Size(); i++) {
    if (i == 0) {  // The updated component data
      EXPECT_EQ(transformComponentVector->At(i)->a, 500);
      EXPECT_EQ(transformComponentVector->At(i)->b, 500);
    } else if (i == 1) {  // The default component values
      EXPECT_EQ(transformComponentVector->At(i)->a, 0);
      EXPECT_EQ(transformComponentVector->At(i)->b, 0);
    }
  }

  // Test component vector size
  // std::vector<void*> imageComponents = registry->getComponents<Image>();
  // ComponentVector<Image>* imageComponentVector =
  //     static_cast<ComponentVector<Image>*>(imageComponents[0]);
  // EXPECT_EQ(imageComponentVector->size(), 1);

  // // Test component vector iteration
  // for (size_t i = 0; i < imageComponentVector->size(); i++) {
  //   // There are multiple members of this struct, just testing these would be
  //   // enough for now.
  //   if (i == 0) {
  //     EXPECT_EQ(imageComponentVector->at(i)->spriteX, 0);
  //     EXPECT_EQ(imageComponentVector->at(i)->spriteY, 0);
  //   }
  // }
}

/*
   When creating entities, there the default values of the components are
   created. This test will test that the default values are created.
 */
TEST(ECS, DefaultValues) {
  Registry* registry = new Registry();

  // Create some archetypes
  registry->RegisterArchetype<TestAddStruct1>();

  // Get some entities
  Entity entity1 = registry->GetAvailableEntityId();

  // Create the entity
  registry->CreateEntity<TestAddStruct1>(entity1);

  // Get the transform component of the entity
  TestAddStruct1* transformComponent = registry->GetComponent<TestAddStruct1>(entity1);

  // Do some assertions that of the default values.
  EXPECT_EQ(transformComponent->a, 0);
  EXPECT_EQ(transformComponent->b, 0);
}

/*
   More of a continuation of the DefaultValues test, this test checks that the
   component data is updated when new component data is added onto it.
 */
TEST(ECS, AddComponentData) {
  Registry* registry = new Registry();

  // Create some archetypes
  registry->RegisterArchetype<TestAddStruct1>();

  // Get some entities
  Entity entity1 = registry->GetAvailableEntityId();

  // Create the entity
  registry->CreateEntity<TestAddStruct1>(entity1);

  // The new transform component
  TestAddStruct1 updatedComponent;
  updatedComponent.a = 500;
  updatedComponent.b = 500;

  registry->AddComponentData<TestAddStruct1>(entity1, updatedComponent);

  // Get the transform component of the entity
  TestAddStruct1* transformComponent = registry->GetComponent<TestAddStruct1>(entity1);

  // Do some assertions that of the default values.
  EXPECT_EQ(transformComponent->a, 500);
  EXPECT_EQ(transformComponent->b, 500);
}

TEST(ECS, GetComponentExact) {
  Registry* registry = new Registry();

  // Create some archetypes
  registry->RegisterArchetype<TestAddStruct1>();
  registry->RegisterArchetype<TestAddStruct1, TestAddStruct2>();

  // Get some entities
  Entity entity1 = registry->GetAvailableEntityId();
  Entity entity2 = registry->GetAvailableEntityId();
  Entity entity3 = registry->GetAvailableEntityId();

  // Create some entities
  registry->CreateEntity<TestAddStruct1>(entity1);
  registry->CreateEntity<TestAddStruct1>(entity2);
  registry->CreateEntity<TestAddStruct1, TestAddStruct2>(entity3);

  std::vector<void*> transformComponents = registry->GetComponentsExact<TestAddStruct1>();
  ComponentVector<TestAddStruct1>* transformComponentVector =
      static_cast<ComponentVector<TestAddStruct1>*>(transformComponents[0]);
  EXPECT_EQ(transformComponentVector->Size(), 2);

  std::vector<void*> transformTestAddStruct2Components =
      registry->GetComponentsExact<TestAddStruct1, TestAddStruct2>();
  ComponentVector<TestAddStruct1>* transformImageComponentVector =
      static_cast<ComponentVector<TestAddStruct1>*>(transformTestAddStruct2Components[0]);
  ComponentVector<TestAddStruct2>* animationComponentVector =
      static_cast<ComponentVector<TestAddStruct2>*>(transformTestAddStruct2Components[1]);
  EXPECT_EQ(transformImageComponentVector->Size(), 1);
  EXPECT_EQ(animationComponentVector->Size(), 1);
}
