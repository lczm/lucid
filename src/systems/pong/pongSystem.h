#pragma once

#include "gl.h"
#include "ecs.h"

class PongSystem : public System {
 private:
 public:
  PongSystem();
  ~PongSystem();

  void Update(double dt, Registry* registry, Input* input);
};