#pragma once

#include "gl.h"
#include "ecs.h"
#include "model.h"

/*
 * What this system does compared to the other systems is that... this is mainly
 * for debugging and a relatively easy sandbox to use when trying out new
 * stuff.
 */
class LucidSystem : public System {
 private:
 public:
  LucidSystem();
  ~LucidSystem();

  void Update(double dt, Registry* registry, Input* input);
};