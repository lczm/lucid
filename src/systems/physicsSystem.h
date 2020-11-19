#pragma once

#include "gl.h"
#include "ecs.h"
#include "cube.h"
#include "sphere.h"
#include "model.h"
#include "gtx/string_cast.hpp"

class PhysicsSystem : public System {
 private:
 public:
  PhysicsSystem();
  ~PhysicsSystem();

  void Update(double dt, Registry* registry, Input* input);

  void CalculateBoundingBox(Cube& const cube, Transform& const transform, RigidBody& rigidBody);
  void CalculateBoundingBox(Sphere& const sphere, Transform& const transform, RigidBody& rigidBody);
  void CalculateBoundingBox(Model& const model, Transform& const transform, RigidBody& rigidBody);
};