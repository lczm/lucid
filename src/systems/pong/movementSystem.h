#pragma once

#include "gl.h"
#include "ecs.h"
#include "cube.h"
#include "sphere.h"
#include "pong.h"

class MovementSystem : public System {
 private:
 public:
  MovementSystem();
  ~MovementSystem();

  void Update(double dt, Registry* registry, Input* input);
};