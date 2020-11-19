#pragma once

#include "gl.h"
#include "ecs.h"
#include "pong.h"

class PlayerSystem : public System {
 private:
 public:
  PlayerSystem();
  ~PlayerSystem();

  void Update(double dt, Registry* registry, Input* input);
};