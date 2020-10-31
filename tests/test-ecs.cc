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

TEST(ECS, ComponentVectorSize) {
  Registry* registry = new Registry();

  // Create some archetypes
  registry->RegisterArchetype<Transform>();
  registry->RegisterArchetype<Transform, Animation>();

  // Get some entities
  Entity entity1 = registry->GetAvailableEntityId();
  Entity entity2 = registry->GetAvailableEntityId();

  // Create the entities
  registry->CreateEntity<Transform>(entity1);
  registry->CreateEntity<Transform, Animation>(entity2);

  // Test transform component size
  std::vector<void*> transformComponents = registry->GetComponents<Transform>();
  ComponentVector<Transform>* transformComponentVector =
      static_cast<ComponentVector<Transform>*>(transformComponents[0]);
  EXPECT_EQ(transformComponentVector->Size(), 2);

  // Test image component size
  std::vector<void*> imageComponents = registry->GetComponents<Animation>();
  ComponentVector<Animation>* imageComponentVector =
      static_cast<ComponentVector<Animation>*>(imageComponents[0]);
  EXPECT_EQ(imageComponentVector->Size(), 1);

  // Create another entity
  Entity entity3 = registry->GetAvailableEntityId();
  registry->CreateEntity<Transform>(entity3);

  // Check that the size of the component vector for Transform is 3 now.
  transformComponents = registry->GetComponents<Transform>();
  transformComponentVector =
      static_cast<ComponentVector<Transform>*>(transformComponents[0]);
  EXPECT_EQ(transformComponentVector->Size(), 3);
}

/*
   Test the iteration of the componentVector
 */
TEST(ECS, ComponentVectorIteration) {
  Registry* registry = new Registry();

  // Create some archetypes
  registry->RegisterArchetype<Transform>();
  registry->RegisterArchetype<Transform, Animation>();

  // Get some entities
  Entity entity1 = registry->GetAvailableEntityId();
  Entity entity2 = registry->GetAvailableEntityId();

  // Create the entities
  registry->CreateEntity<Transform>(entity1);
  registry->CreateEntity<Transform, Animation>(entity2);

  Transform updatedComponent;
  updatedComponent.x = 500;
  updatedComponent.y = 500;
  updatedComponent.scale = 5;

  registry->AddComponentData<Transform>(entity1, updatedComponent);

  // Test component vector size
  std::vector<void*> transformComponents = registry->GetComponents<Transform>();
  ComponentVector<Transform>* transformComponentVector =
      static_cast<ComponentVector<Transform>*>(transformComponents[0]);
  EXPECT_EQ(transformComponentVector->Size(), 2);

  // Test component vector iteration
  for (size_t i = 0; i < transformComponentVector->Size(); i++) {
    if (i == 0) {  // The updated component data
      EXPECT_EQ(transformComponentVector->At(i)->x, 500);
      EXPECT_EQ(transformComponentVector->At(i)->y, 500);
      EXPECT_EQ(transformComponentVector->At(i)->scale, 5);
    } else if (i == 1) {  // The default component values
      EXPECT_EQ(transformComponentVector->At(i)->x, 0);
      EXPECT_EQ(transformComponentVector->At(i)->y, 0);
      EXPECT_EQ(transformComponentVector->At(i)->scale, 1);
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
  registry->RegisterArchetype<Transform>();

  // Get some entities
  Entity entity1 = registry->GetAvailableEntityId();

  // Create the entity
  registry->CreateEntity<Transform>(entity1);

  // Get the transform component of the entity
  Transform* transformComponent = registry->GetComponent<Transform>(entity1);

  // Do some assertions that of the default values.
  EXPECT_EQ(transformComponent->x, 0);
  EXPECT_EQ(transformComponent->y, 0);
  EXPECT_EQ(transformComponent->scale, 1);
}

/*
   More of a continuation of the DefaultValues test, this test checks that the
   component data is updated when new component data is added onto it.
 */
TEST(ECS, AddComponentData) {
  Registry* registry = new Registry();

  // Create some archetypes
  registry->RegisterArchetype<Transform>();

  // Get some entities
  Entity entity1 = registry->GetAvailableEntityId();

  // Create the entity
  registry->CreateEntity<Transform>(entity1);

  // The new transform component
  Transform updatedComponent;
  updatedComponent.x = 500;
  updatedComponent.y = 500;
  updatedComponent.scale = 10;

  registry->AddComponentData<Transform>(entity1, updatedComponent);

  // Get the transform component of the entity
  Transform* transformComponent = registry->GetComponent<Transform>(entity1);

  // Do some assertions that of the default values.
  EXPECT_EQ(transformComponent->x, 500);
  EXPECT_EQ(transformComponent->y, 500);
  EXPECT_EQ(transformComponent->scale, 10);
}

TEST(ECS, GetComponentExact) {
  Registry* registry = new Registry();

  // Create some archetypes
  registry->RegisterArchetype<Transform>();
  registry->RegisterArchetype<Transform, Animation>();

  // Get some entities
  Entity entity1 = registry->GetAvailableEntityId();
  Entity entity2 = registry->GetAvailableEntityId();
  Entity entity3 = registry->GetAvailableEntityId();

  // Create some entities
  registry->CreateEntity<Transform>(entity1);
  registry->CreateEntity<Transform>(entity2);
  registry->CreateEntity<Transform, Animation>(entity3);

  std::vector<void*> transformComponents =
      registry->GetComponentsExact<Transform>();
  ComponentVector<Transform>* transformComponentVector =
      static_cast<ComponentVector<Transform>*>(transformComponents[0]);
  EXPECT_EQ(transformComponentVector->Size(), 2);

  std::vector<void*> transformAnimationComponents =
      registry->GetComponentsExact<Transform, Animation>();
  ComponentVector<Transform>* transformImageComponentVector =
      static_cast<ComponentVector<Transform>*>(transformAnimationComponents[0]);
  ComponentVector<Animation>* animationComponentVector =
      static_cast<ComponentVector<Animation>*>(transformAnimationComponents[1]);
  EXPECT_EQ(transformImageComponentVector->Size(), 1);
  EXPECT_EQ(animationComponentVector->Size(), 1);
}
