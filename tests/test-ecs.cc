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
  registry->registerArchetype<Transform>();
  registry->registerArchetype<Transform, Image>();

  // Get some entities
  Entity entity1 = registry->getAvailableEntityId();
  Entity entity2 = registry->getAvailableEntityId();

  // Create the entities
  registry->createEntity<Transform>(entity1);
  registry->createEntity<Transform, Image>(entity2);

  // Test transform component size
  std::vector<void*> transformComponents = registry->getComponents<Transform>();
  ComponentVector<Transform>* transformComponentVector =
      static_cast<ComponentVector<Transform>*>(transformComponents[0]);
  EXPECT_EQ(transformComponentVector->size(), 2);

  // Test image component size
  std::vector<void*> imageComponents = registry->getComponents<Image>();
  ComponentVector<Image>* imageComponentVector =
      static_cast<ComponentVector<Image>*>(imageComponents[0]);
  EXPECT_EQ(imageComponentVector->size(), 1);

  // Create another entity
  Entity entity3 = registry->getAvailableEntityId();
  registry->createEntity<Transform>(entity3);

  // Check that the size of the component vector for Transform is 3 now.
  transformComponents = registry->getComponents<Transform>();
  transformComponentVector =
      static_cast<ComponentVector<Transform>*>(transformComponents[0]);
  EXPECT_EQ(transformComponentVector->size(), 3);
}

/*
   Test the iteration of the componentVector
 */
TEST(ECS, ComponentVectorIteration) {
  Registry* registry = new Registry();

  // Create some archetypes
  registry->registerArchetype<Transform>();
  registry->registerArchetype<Transform, Image>();

  // Get some entities
  Entity entity1 = registry->getAvailableEntityId();
  Entity entity2 = registry->getAvailableEntityId();

  // Create the entities
  registry->createEntity<Transform>(entity1);
  registry->createEntity<Transform, Image>(entity2);

  Transform updatedComponent;
  updatedComponent.x = 500;
  updatedComponent.y = 500;
  updatedComponent.scale = 5;

  registry->addComponentData<Transform>(entity1, updatedComponent);

  // Test component vector size
  std::vector<void*> transformComponents = registry->getComponents<Transform>();
  ComponentVector<Transform>* transformComponentVector =
      static_cast<ComponentVector<Transform>*>(transformComponents[0]);
  EXPECT_EQ(transformComponentVector->size(), 2);

  // Test component vector iteration
  for (size_t i = 0; i < transformComponentVector->size(); i++) {
    if (i == 0) {  // The updated component data
      EXPECT_EQ(transformComponentVector->at(i)->x, 500);
      EXPECT_EQ(transformComponentVector->at(i)->y, 500);
      EXPECT_EQ(transformComponentVector->at(i)->scale, 5);
    } else if (i == 1) {  // The default component values
      EXPECT_EQ(transformComponentVector->at(i)->x, 0);
      EXPECT_EQ(transformComponentVector->at(i)->y, 0);
      EXPECT_EQ(transformComponentVector->at(i)->scale, 1);
    }
  }

  // Test component vector size
  std::vector<void*> imageComponents = registry->getComponents<Image>();
  ComponentVector<Image>* imageComponentVector =
      static_cast<ComponentVector<Image>*>(imageComponents[0]);
  EXPECT_EQ(imageComponentVector->size(), 1);

  // Test component vector iteration
  for (size_t i = 0; i < imageComponentVector->size(); i++) {
    // There are multiple members of this struct, just testing these would be
    // enough for now.
    if (i == 0) {
      EXPECT_EQ(imageComponentVector->at(i)->spriteX, 0);
      EXPECT_EQ(imageComponentVector->at(i)->spriteY, 0);
    }
  }
}

/*
   When creating entities, there the default values of the components are
   created. This test will test that the default values are created.
 */
TEST(ECS, DefaultValues) {
  Registry* registry = new Registry();

  // Create some archetypes
  registry->registerArchetype<Transform>();

  // Get some entities
  Entity entity1 = registry->getAvailableEntityId();

  // Create the entity
  registry->createEntity<Transform>(entity1);

  // Get the transform component of the entity
  Transform* transformComponent = registry->getComponent<Transform>(entity1);

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
  registry->registerArchetype<Transform>();

  // Get some entities
  Entity entity1 = registry->getAvailableEntityId();

  // Create the entity
  registry->createEntity<Transform>(entity1);

  // The new transform component
  Transform updatedComponent;
  updatedComponent.x = 500;
  updatedComponent.y = 500;
  updatedComponent.scale = 10;

  registry->addComponentData<Transform>(entity1, updatedComponent);

  // Get the transform component of the entity
  Transform* transformComponent = registry->getComponent<Transform>(entity1);

  // Do some assertions that of the default values.
  EXPECT_EQ(transformComponent->x, 500);
  EXPECT_EQ(transformComponent->y, 500);
  EXPECT_EQ(transformComponent->scale, 10);
}

TEST(ECS, GetComponentExact) {
  Registry* registry = new Registry();

  // Create some archetypes
  registry->registerArchetype<Transform>();
  registry->registerArchetype<Transform, Image>();

  // Get some entities
  Entity entity1 = registry->getAvailableEntityId();
  Entity entity2 = registry->getAvailableEntityId();
  Entity entity3 = registry->getAvailableEntityId();

  // Create some entities
  registry->createEntity<Transform>(entity1);
  registry->createEntity<Transform>(entity2);
  registry->createEntity<Transform, Image>(entity3);

  std::vector<void*> transformComponents =
      registry->getComponentsExact<Transform>();
  ComponentVector<Transform>* transformComponentVector =
      static_cast<ComponentVector<Transform>*>(transformComponents[0]);
  EXPECT_EQ(transformComponentVector->size(), 2);

  std::vector<void*> transformImageComponents =
      registry->getComponentsExact<Transform, Image>();
  ComponentVector<Transform>* transformImageComponentVector =
      static_cast<ComponentVector<Transform>*>(transformImageComponents[0]);
  ComponentVector<Image>* imageComponentVector =
      static_cast<ComponentVector<Image>*>(transformImageComponents[1]);
  EXPECT_EQ(transformImageComponentVector->size(), 1);
  EXPECT_EQ(imageComponentVector->size(), 1);
}
