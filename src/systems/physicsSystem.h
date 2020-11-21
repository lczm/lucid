#pragma once

#include "gl.h"
#include "ecs.h"
#include "cube.h"
#include "sphere.h"
#include "model.h"
#include "gtx/string_cast.hpp"

// A box without rotation (axis-aligned)
struct AxisAlignedBoundingBox {};

struct OrientedBoundingBox {};

class PhysicsSystem : public System {
 private:
 public:
  PhysicsSystem();
  ~PhysicsSystem();

  void Update(double dt, Registry* registry, Input* input);

  void CheckCollision(BoundingBoxCube& boundingBoxCube, Transform& transform,
                      BoundingBoxCube& boundingBoxCubeOther, Transform& transformOther);

  void GetAxisAlignedBoundingBox(ColliderSphere& collider, Transform& transform);
  void GetOrientedBoundingBox(ColliderSphere& collider, Transform& transform);
};