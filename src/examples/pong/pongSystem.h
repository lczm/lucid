#pragma once

#include "gl.h"
#include "ecs.h"
#include "sphere.h"

class PongSystem : public System
{
 private:
 public:
  PongSystem();
  ~PongSystem();

  void Update(float dt, Registry* registry, Input* input);
};