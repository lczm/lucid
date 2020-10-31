#include <vector>
#include "ecs.h"
#include "component.h"
#include "gtest/gtest.h"

// TEST(ECS, AddComponent) {
//   Registry* registry = new Registry();
//
//   // Create some archetypes
//   registry->RegisterArchetype<Transform>();
//
//   // Create some entities
//   Entity entity1 = registry->GetAvailableEntityId();
//   registry->CreateEntity<Transform>(entity1);
//
//   auto components = registry->GetComponents<Transform>();
//   auto* transformComponentVector = static_cast<ComponentVector<Transform>*>(components[0]);
//
//   // There should only be one transform component that exists
//   EXPECT_EQ(transformComponentVector->Size(), 1);
//
//   // Add an animation component to entity1
//   registry->AddComponent<Animation>(entity1);
//
//   components = registry->GetComponents<Transform>();
//   transformComponentVector = static_cast<ComponentVector<Transform>*>(components[0]);
//
//   // There should still only be one transform component that exists
//   EXPECT_EQ(transformComponentVector->Size(), 1);
//
//   components = registry->GetComponents<Transform, Animation>();
//   transformComponentVector = static_cast<ComponentVector<Transform>*>(components[0]);
//   auto* animationComponentVector = static_cast<ComponentVector<Animation>*>(components[1]);
//
//   // Now there should be an animation componentVector with it as well.
//   EXPECT_EQ(transformComponentVector->Size(), 1);
//   EXPECT_EQ(animationComponentVector->Size(), 1);
// }
//
// TEST(ECS, RemoveComponent) {
// }