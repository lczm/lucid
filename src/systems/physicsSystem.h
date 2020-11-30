#pragma once

#include "gl.h"
#include "ecs.h"
#include "cube.h"
#include "sphere.h"
#include "model.h"
#include "gtx/string_cast.hpp"

struct AxisAlignedBoundingBox {};
struct OrientedBoundingBox {};

class PhysicsSystem : public System {
 private:
 public:
  PhysicsSystem();
  ~PhysicsSystem();

  void Update(double dt, Registry* registry, Input* input);

  bool CheckCollision(BoundingBoxCube& boundingBoxCube, Transform& transform,
                      BoundingBoxCube& boundingBoxCubeOther, Transform& transformOther);
  bool CheckCollision(BoundingBoxCube& boundingBoxCube, Transform& transform,
                      BoundingBoxSphere& boundingBoxSphereOther, Transform& transformOther);
  bool CheckCollision(BoundingBoxCube& boundingBoxCube, Transform& transform,
                      BoundingBoxPolygon& boundingBoxPolygonOther, Transform& transformOther);

  bool CheckCollision(BoundingBoxSphere& boundingBoxSphere, Transform& transform,
                      BoundingBoxSphere& boundingBoxSphereOther, Transform& transformOther);
  bool CheckCollision(BoundingBoxSphere& boundingBoxSphere, Transform& transform,
                      BoundingBoxCube& boundingBoxCubeOther, Transform& transformOther);
  bool CheckCollision(BoundingBoxSphere& boundingBoxSphere, Transform& transform,
                      BoundingBoxPolygon& boundingBoxPolygonOther, Transform& transformOther);

  bool CheckCollision(BoundingBoxPolygon& boundingBoxPolygon, Transform& transform,
                      BoundingBoxPolygon& boundingBoxPolygonOther, Transform& transformOther);
  bool CheckCollision(BoundingBoxPolygon& boundingBoxPolygon, Transform& transform,
                      BoundingBoxCube& boundingBoxCubeOther, Transform& transformOther);
  bool CheckCollision(BoundingBoxPolygon& boundingBoxPolygon, Transform& transform,
                      BoundingBoxSphere& boundingBoxSphereOther, Transform& transformOther);

  glm::mat4 PhysicsSystem::ApplyTransformation(Transform& transform);
  bool CheckCollisionBetweenBoundingBox(BoundingBox boundingBox, BoundingBox boundingBoxOther);

  void GetAxisAlignedBoundingBox(ColliderSphere& collider, Transform& transform);
  void GetOrientedBoundingBox(ColliderSphere& collider, Transform& transform);
};