#pragma once

#include "ecs.h"
#include "ray.h"
#include "colliders.h"
#include "renderUtils.h"
#include "gtc/matrix_transform.hpp"
#include <iostream>
#include "sound.h"
#include "font.h"
#include "rules.h"
#include "model.h"

class FpsSystem : public System
{
 private:
  int numEnemies = 10;

 public:
  FpsSystem();
  ~FpsSystem();

  void Update(float dt, Registry* registry, Input* input);
};