#pragma once

#include "gl.h"
#include "ecs.h"

class PlayerSystem : public System {
 private:
 public:
  PlayerSystem();
  ~PlayerSystem();

  void Update(double dt, Registry* registry, Input* input);
};