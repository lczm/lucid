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

class FpsSystem : public System
{
 public:
  FpsSystem();
  ~FpsSystem();

  void Update(float dt, Registry* registry, Input* input);
};