#pragma once

#include <iostream>

#include "ecs.h"

class TestSystem : public System {
public:
  TestSystem();
  ~TestSystem();

  void Update(float dt, Registry *registry, Input *input);
};