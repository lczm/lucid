#pragma once

#include "shader.h"
#include "ecs.h"
#include "pong.h"
#include "gtx/string_cast.hpp"

class PlayerSystem : public System
{
 private:
 public:
  PlayerSystem();
  ~PlayerSystem();

  void Update(float dt, Registry* registry, Input* input);
};