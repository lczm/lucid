#pragma once

#include "ecs.h"
#include "ray.h"
#include "renderUtils.h"
#include "gtc/matrix_transform.hpp"
#include <iostream>

class FpsSystem : public System
{
 public:
  FpsSystem();
  ~FpsSystem();

  void Update(float dt, Registry* registry, Input* input);
};