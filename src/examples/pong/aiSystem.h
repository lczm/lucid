#pragma once

#include "shader.h"
#include "ecs.h"
#include "pong.h"

class AiSystem : public System
{
 private:
 public:
  AiSystem();
  ~AiSystem();

  void Update(float dt, Registry* registry, Input* input);
};