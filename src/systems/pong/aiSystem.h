#pragma once

#include "gl.h"
#include "ecs.h"
#include "pong.h"

class AiSystem : public System {
 private:
 public:
  AiSystem();
  ~AiSystem();

  void Update(double dt, Registry* registry, Input* input);
};