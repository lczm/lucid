#pragma once

#include "gl.h"
#include "ecs.h"
#include "pong.h"
#include "gtx/string_cast.hpp"

class PlayerSystem : public System {
 private:
 public:
  PlayerSystem();
  ~PlayerSystem();

  void Update(double dt, Registry* registry, Input* input);
};