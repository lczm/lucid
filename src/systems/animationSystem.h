#pragma once

#include <iostream>

#include "glm.hpp"
#include "gtc/type_ptr.hpp"

#include "ecs.h"
#include "component.h"
#include "engineComponents.h"
#include "model.h"
#include "shader.h"
#include "shaderResource.h"

class AnimationSystem : public System
{
 public:
  AnimationSystem();
  ~AnimationSystem();

  void Update(float dt, Registry* registry, Input* input);

  void SetBoneTransform(float dt, Registry* registry, Model& model);
  void BoneTransform(float dt, std::vector<glm::mat4> transforms);
};