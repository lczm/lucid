#include "testSystem.h"

TestSystem::TestSystem() {}

TestSystem::~TestSystem() {}

void TestSystem::Update(float dt, Registry *registry, Input *input) {
  std::cout << "TestSystem" << std::endl;

  // registry->GetComponentsIter<RigidBody>()->Each([&](RigidBody& rigidBody)
  // {
  //   rigidBody.velocity.z = 10.0f;
  // });
}