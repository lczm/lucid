// This is needed becuase component imports texture which uses stbi etc...
#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION

#include <vector>
#include "ecs.h"
#include "component.h"
#include "test-utils.h"
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

struct TestAddStruct3 {
  int e = 0;
  int f = 0;
};

struct TestAddStructDynamic1 {
  int a;
};

struct TestAddStructDynamic2 {
  float b;
};

class EcsGL : public testing::Test {
 protected:
  static GLFWwindow* window;
  static void SetUpTestSuite() {
    window = SetUpWindow();
  }

  static void TearDownTestSuite() {
    TearDownWindow(window);
  }
};

GLFWwindow* EcsGL::window = nullptr;

TEST_F(EcsGL, ComponentVectorSize) {
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
  ASSERT_EQ(transformComponentVector->Size(), 2);

  // Test image component size
  std::vector<void*> imageComponents = registry->GetComponents<TestAddStruct2>();
  ComponentVector<TestAddStruct2>* imageComponentVector =
      static_cast<ComponentVector<TestAddStruct2>*>(imageComponents[0]);
  ASSERT_EQ(imageComponentVector->Size(), 1);

  // Create another entity
  Entity entity3 = registry->GetAvailableEntityId();
  registry->CreateEntity<TestAddStruct1>(entity3);

  // Check that the size of the component vector for TestAddStruct1 is 3 now.
  transformComponents = registry->GetComponents<TestAddStruct1>();
  transformComponentVector = static_cast<ComponentVector<TestAddStruct1>*>(transformComponents[0]);
  ASSERT_EQ(transformComponentVector->Size(), 3);
}

/*
   Test the iteration of the componentVector
 */
TEST_F(EcsGL, ComponentVectorIteration) {
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
  ASSERT_EQ(transformComponentVector->Size(), 2);

  // Test component vector iteration
  for (size_t i = 0; i < transformComponentVector->Size(); i++) {
    if (i == 0) {  // The updated component data
      ASSERT_EQ(transformComponentVector->At(i).a, 500);
      ASSERT_EQ(transformComponentVector->At(i).b, 500);
    } else if (i == 1) {  // The default component values
      ASSERT_EQ(transformComponentVector->At(i).a, 0);
      ASSERT_EQ(transformComponentVector->At(i).b, 0);
    }
  }
}

TEST_F(EcsGL, ComponentVectorIterationIndex) {
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

  std::vector<void*> components = registry->GetComponents<TestAddStruct1>();
  auto* testAddStructComponentVector = static_cast<ComponentVector<TestAddStruct1>*>(components[0]);

  ASSERT_EQ(testAddStructComponentVector->Size(), 2);

  ComponentVector<TestAddStruct1>& a = *(testAddStructComponentVector);

  for (size_t i = 0; i < testAddStructComponentVector->Size(); i++) {
    TestAddStruct1 testAddStruct = a[i];
    ASSERT_EQ(testAddStruct.a, 0);
    ASSERT_EQ(testAddStruct.b, 0);
  }
}

/*
   When creating entities, there the default values of the components are
   created. This test will test that the default values are created.
 */
TEST_F(EcsGL, DefaultValues) {
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
  ASSERT_EQ(transformComponent->a, 0);
  ASSERT_EQ(transformComponent->b, 0);
}

/*
   More of a continuation of the DefaultValues test, this test checks that the
   component data is updated when new component data is added onto it.
 */
TEST_F(EcsGL, AddComponentData) {
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
  ASSERT_EQ(transformComponent->a, 500);
  ASSERT_EQ(transformComponent->b, 500);
}

TEST_F(EcsGL, GetComponentExact) {
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
  ASSERT_EQ(transformComponentVector->Size(), 2);

  std::vector<void*> transformTestAddStruct2Components =
      registry->GetComponentsExact<TestAddStruct1, TestAddStruct2>();
  ComponentVector<TestAddStruct1>* transformImageComponentVector =
      static_cast<ComponentVector<TestAddStruct1>*>(transformTestAddStruct2Components[0]);
  ComponentVector<TestAddStruct2>* animationComponentVector =
      static_cast<ComponentVector<TestAddStruct2>*>(transformTestAddStruct2Components[1]);
  ASSERT_EQ(transformImageComponentVector->Size(), 1);
  ASSERT_EQ(animationComponentVector->Size(), 1);
}

TEST_F(EcsGL, GetComponentSingle) {
  Registry* registry = new Registry();

  // Create one archetype
  registry->RegisterArchetype<TestAddStruct1>();

  // Get some entities
  Entity entity1 = registry->GetAvailableEntityId();

  // Create some entities
  registry->CreateEntity<TestAddStruct1>(entity1);
  registry->AddComponentData<TestAddStruct1>(entity1, {10, 20});

  // Get the struct
  TestAddStruct1 testStruct = registry->GetComponent<TestAddStruct1>();

  ASSERT_EQ(testStruct.a, 10);
  ASSERT_EQ(testStruct.b, 20);
}

TEST_F(EcsGL, GetComponentSingleAndModify) {
  Registry* registry = new Registry();

  // Create one archetype
  registry->RegisterArchetype<TestAddStruct1>();

  // Get some entities
  Entity entity1 = registry->GetAvailableEntityId();

  // Create some entities
  registry->CreateEntity<TestAddStruct1>(entity1);
  registry->AddComponentData<TestAddStruct1>(entity1, {10, 20});

  // Get the struct
  TestAddStruct1 testStruct = registry->GetComponent<TestAddStruct1>();

  ASSERT_EQ(testStruct.a, 10);
  ASSERT_EQ(testStruct.b, 20);

  testStruct.a = 500;
  testStruct.b = 1000;

  TestAddStruct1 testStructModified = registry->GetComponent<TestAddStruct1>();

  ASSERT_EQ(testStruct.a, 500);
  ASSERT_EQ(testStruct.b, 1000);
}

TEST_F(EcsGL, GetComponentsLambdaSingleIteration) {
  Registry* registry = new Registry();

  // Create a few archetypes
  registry->RegisterArchetype<TestAddStruct1>();
  registry->RegisterArchetype<TestAddStruct1, TestAddStruct2>();

  // Get some entities
  Entity entity1 = registry->GetAvailableEntityId();
  Entity entity2 = registry->GetAvailableEntityId();
  Entity entity3 = registry->GetAvailableEntityId();
  Entity entity4 = registry->GetAvailableEntityId();

  // Create some entities
  registry->CreateEntity<TestAddStruct1>(entity1);
  registry->CreateEntity<TestAddStruct1>(entity2);
  registry->CreateEntity<TestAddStruct1, TestAddStruct2>(entity3);
  registry->CreateEntity<TestAddStruct1, TestAddStruct2>(entity4);

  registry->AddComponentData<TestAddStruct1>(entity1, {10, 10});

  const double dt = 0.001;

  std::vector<uint32_t> testAData = {10, 0, 0, 0};
  std::vector<uint32_t> testBData = {10, 0, 0, 0};

  std::vector<uint32_t> testAData2 = {11, 1, 1, 1};
  std::vector<uint32_t> testBData2 = {11, 1, 1, 1};

  uint32_t i = 0;
  registry->GetComponentsIter<TestAddStruct1>()->Each(
      [dt, &i, &testAData, &testBData](TestAddStruct1& testAddStruct) {
        ASSERT_EQ(dt, 0.001);
        ASSERT_EQ(testAddStruct.a, testAData[i]);
        ASSERT_EQ(testAddStruct.b, testBData[i]);

        i++;

        testAddStruct.a++;
        testAddStruct.b++;
      });

  i = 0;
  registry->GetComponentsIter<TestAddStruct1>()->Each(
      [dt, &i, &testAData2, &testBData2](TestAddStruct1& testAddStruct) {
        ASSERT_EQ(dt, 0.001);
        ASSERT_EQ(testAddStruct.a, testAData2[i]);
        ASSERT_EQ(testAddStruct.b, testBData2[i]);

        i++;
      });
}

TEST_F(EcsGL, GetComponentsLambdaMultiIteration) {
  Registry* registry = new Registry();

  // Create a few archetypes
  registry->RegisterArchetype<TestAddStruct1>();
  registry->RegisterArchetype<TestAddStruct1, TestAddStruct2>();
  registry->RegisterArchetype<TestAddStruct1, TestAddStruct2, TestAddStruct3>();

  // Get some entities
  Entity entity1 = registry->GetAvailableEntityId();
  Entity entity2 = registry->GetAvailableEntityId();
  Entity entity3 = registry->GetAvailableEntityId();
  Entity entity4 = registry->GetAvailableEntityId();
  Entity entity5 = registry->GetAvailableEntityId();
  Entity entity6 = registry->GetAvailableEntityId();

  // Create some entities
  registry->CreateEntity<TestAddStruct1>(entity1);
  registry->CreateEntity<TestAddStruct1>(entity2);
  registry->CreateEntity<TestAddStruct1, TestAddStruct2>(entity3);
  registry->CreateEntity<TestAddStruct1, TestAddStruct2>(entity4);
  registry->CreateEntity<TestAddStruct1, TestAddStruct2, TestAddStruct3>(entity5);
  registry->CreateEntity<TestAddStruct1, TestAddStruct2, TestAddStruct3>(entity6);

  const double dt = 0.001;

  std::vector<uint32_t> testAData = {0, 0, 0, 0};
  std::vector<uint32_t> testBData = {0, 0, 0, 0};

  std::vector<uint32_t> testCData = {0, 0, 0, 0};
  std::vector<uint32_t> testDData = {0, 0, 0, 0};

  std::vector<uint32_t> testEData = {0, 0, 0, 0};
  std::vector<uint32_t> testFData = {0, 0, 0, 0};

  // After modification
  std::vector<uint32_t> testAData2 = {1, 1, 1, 1};
  std::vector<uint32_t> testBData2 = {1, 1, 1, 1};

  std::vector<uint32_t> testCData2 = {1, 1, 1, 1};
  std::vector<uint32_t> testDData2 = {1, 1, 1, 1};

  uint32_t i = 0;
  registry->GetComponentsIter<TestAddStruct1, TestAddStruct2>()->Each(
      [dt, &i, &testAData, &testBData, &testCData, &testDData](TestAddStruct1& testAddStruct,
                                                               TestAddStruct2& testAddStruct2) {
        ASSERT_EQ(dt, 0.001);
        ASSERT_EQ(testAddStruct.a, testAData[i]);
        ASSERT_EQ(testAddStruct.b, testBData[i]);
        ASSERT_EQ(testAddStruct2.c, testCData[i]);
        ASSERT_EQ(testAddStruct2.d, testDData[i]);

        testAddStruct.a++;
        testAddStruct.b++;
        testAddStruct2.c++;
        testAddStruct2.d++;

        i++;
      });

  i = 0;
  registry->GetComponentsIter<TestAddStruct1, TestAddStruct2>()->Each(
      [dt, &i, &testAData2, &testBData2, &testCData2, &testDData2](TestAddStruct1& testAddStruct,
                                                                   TestAddStruct2& testAddStruct2) {
        ASSERT_EQ(dt, 0.001);
        ASSERT_EQ(testAddStruct.a, testAData2[i]);
        ASSERT_EQ(testAddStruct.b, testBData2[i]);
        ASSERT_EQ(testAddStruct2.c, testCData2[i]);
        ASSERT_EQ(testAddStruct2.d, testDData2[i]);

        i++;
      });
}

TEST_F(EcsGL, GetComponentsLambdaMultiIterationWithInitialDataModified) {
  Registry* registry = new Registry();

  // Create a few archetypes
  registry->RegisterArchetype<TestAddStruct1>();
  registry->RegisterArchetype<TestAddStruct1, TestAddStruct2>();
  registry->RegisterArchetype<TestAddStruct1, TestAddStruct2, TestAddStruct3>();

  // Get some entities
  Entity entity1 = registry->GetAvailableEntityId();
  Entity entity2 = registry->GetAvailableEntityId();
  Entity entity3 = registry->GetAvailableEntityId();

  // Create some entities
  registry->CreateEntity<TestAddStruct1>(entity1);
  registry->CreateEntity<TestAddStruct1>(entity2);
  registry->CreateEntity<TestAddStruct1, TestAddStruct2, TestAddStruct3>(entity3);

  registry->AddComponentData<TestAddStruct1>(entity3, {10, 10});
  registry->AddComponentData<TestAddStruct2>(entity3, {10, 10});

  const double dt = 0.001;

  std::vector<void*> components =
      registry->GetComponents<TestAddStruct1, TestAddStruct2, TestAddStruct3>();
  ComponentVector<TestAddStruct1>* test =
      static_cast<ComponentVector<TestAddStruct1>*>(components[0]);
  ComponentVector<TestAddStruct3>* test2 =
      static_cast<ComponentVector<TestAddStruct3>*>(components[2]);

  TestAddStruct1& data = test->At(0);
  ASSERT_EQ(data.a, 10);
  ASSERT_EQ(data.b, 10);

  TestAddStruct3& data2 = test2->At(0);
  ASSERT_EQ(data2.e, 0);
  ASSERT_EQ(data2.f, 0);

  registry->GetComponentsIter<TestAddStruct1, TestAddStruct2, TestAddStruct3>()->Each(
      [dt](TestAddStruct1& testAddStruct, TestAddStruct2& testAddStruct2,
           TestAddStruct3& testAddStruct3) {
        ASSERT_EQ(dt, 0.001);

        ASSERT_EQ(testAddStruct.a, 10);
        ASSERT_EQ(testAddStruct.b, 10);

        ASSERT_EQ(testAddStruct2.c, 10);
        ASSERT_EQ(testAddStruct2.d, 10);

        ASSERT_EQ(testAddStruct3.e, 0);
        ASSERT_EQ(testAddStruct3.f, 0);
      });
}

TEST_F(EcsGL, ECS_GetComponentsLambdaMultiIterationWithEntityID) {
  Registry* registry = new Registry();

  registry->RegisterArchetype<TestAddStruct1>();

  Entity entity1 = registry->GetAvailableEntityId();
  Entity entity2 = registry->GetAvailableEntityId();
  Entity entity3 = registry->GetAvailableEntityId();

  registry->CreateEntity<TestAddStruct1>(entity1);
  registry->CreateEntity<TestAddStruct1>(entity2);
  registry->CreateEntity<TestAddStruct1>(entity3);

  std::vector<Entity> idCollection;
  idCollection.push_back(entity1);
  idCollection.push_back(entity2);
  idCollection.push_back(entity3);

  int count = 0;
  registry->GetComponentsIter<TestAddStruct1>()->EachWithID(
      [&](Entity id, TestAddStruct1 testAddStruct) {
        ASSERT_EQ(id, idCollection[count]);
        count++;
      });
}

TEST_F(EcsGL, GetEntityIDFromArchetypeWithOneEntity) {
  Registry* registry = new Registry();

  // Register some archetypes
  registry->RegisterArchetype<TestAddStruct1>();
  registry->RegisterArchetype<TestAddStruct1, TestAddStruct2>();
  registry->RegisterArchetype<TestAddStruct1, TestAddStruct2, TestAddStruct3>();

  // Get some entityIDs
  Entity entity1 = registry->GetAvailableEntityId();
  Entity entity2 = registry->GetAvailableEntityId();
  Entity entity3 = registry->GetAvailableEntityId();

  // Create some entities
  registry->CreateEntity<TestAddStruct1>(entity1);
  registry->CreateEntity<TestAddStruct1, TestAddStruct2>(entity2);
  registry->CreateEntity<TestAddStruct1, TestAddStruct2, TestAddStruct3>(entity3);

  // Attempt to find entity1
  // Since there is only one entity in the archetype, this should return entity1
  Entity entityID1 = registry->GetEntityIDFromArchetype<TestAddStruct1>(0);
  Entity entityID2 = registry->GetEntityIDFromArchetype<TestAddStruct1, TestAddStruct2>(0);
  Entity entityID3 =
      registry->GetEntityIDFromArchetype<TestAddStruct1, TestAddStruct2, TestAddStruct3>(0);

  ASSERT_EQ(entity1, entityID1);
  ASSERT_EQ(entity2, entityID2);
  ASSERT_EQ(entity3, entityID3);
}

TEST_F(EcsGL, GetEntityIDFromArchetypeWithMoreThanOneEntity) {
  Registry* registry = new Registry();

  // Register some archetypes
  registry->RegisterArchetype<TestAddStruct1>();

  // Get some entityIDs
  Entity entity1 = registry->GetAvailableEntityId();
  Entity entity2 = registry->GetAvailableEntityId();
  Entity entity3 = registry->GetAvailableEntityId();

  // Create some entities
  registry->CreateEntity<TestAddStruct1>(entity1);
  registry->CreateEntity<TestAddStruct1>(entity2);
  registry->CreateEntity<TestAddStruct1>(entity3);

  // Attempt to find entity1
  // Since there is only one entity in the archetype, this should return entity1
  Entity entityID1 = registry->GetEntityIDFromArchetype<TestAddStruct1>(0);
  Entity entityID2 = registry->GetEntityIDFromArchetype<TestAddStruct1>(1);
  Entity entityID3 = registry->GetEntityIDFromArchetype<TestAddStruct1>(2);

  ASSERT_EQ(entity1, entityID1);
  ASSERT_EQ(entity2, entityID2);
  ASSERT_EQ(entity3, entityID3);
}

TEST_F(EcsGL, AddComponent) {
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

  ASSERT_EQ(transformCV->Size(), 1);
  ASSERT_EQ(testAddStructCV->Size(), 1);

  // Add another component to entity1
  // registry->AddComponent<TestAddStructDynamic2>(entity1);
  // registry->FlushEntity<Transform, TestAddStructDynamic1, TestAddStructDynamic2>(entity1);

  // components = registry->GetComponents<Transform, TestAddStructDynamic1,
  // TestAddStructDynamic2>();

  // transformCV = static_cast<ComponentVector<Transform>*>(components[0]);
  // testAddStructCV = static_cast<ComponentVector<TestAddStructDynamic1>*>(components[1]);
  // auto* testAddStruct2CV = static_cast<ComponentVector<TestAddStructDynamic2>*>(components[2]);

  // ASSERT_EQ(transformCV->Size(), 1);
  // ASSERT_EQ(testAddStructCV->Size(), 1);
  // ASSERT_EQ(testAddStruct2CV->Size(), 1);
}

TEST_F(EcsGL, RemoveComponent) {
}

TEST_F(EcsGL, AddRemoveComponent) {
}