#pragma once

#include "shader.h"
#include "ecs.h"
#include "cube.h"
#include "sphere.h"
#include "model.h"
#include "engineComponents.h"
#include "gtx/string_cast.hpp"

class PhysicsSystem : public System
{
 private:
 public:
  PhysicsSystem();
  ~PhysicsSystem();

  void Update(float dt, Registry* registry, Input* input);

  void UpdateAllRigidbodies(float dt, Registry* registry, Input* input);
  void UpdateCollisions(float dt, Registry* registry, Input* input);

  // Cube : { Rest of the colliders };
  bool CheckCollision(ColliderCube colliderCube, Transform& transform,
                      ColliderCube colliderCubeOther, Transform& transformOther);
  bool CheckCollision(ColliderCube colliderCube, Transform& transform,
                      ColliderSphere colliderSphereOther, Transform& transformOther);
  bool CheckCollision(ColliderCube colliderCube, Transform& transform,
                      ColliderPolygon colliderPolygonOther, Transform& transformOther);

  // Sphere : { Rest of the colliders }
  bool CheckCollision(ColliderSphere colliderSphere, Transform& transform,
                      ColliderSphere colliderSphereOther, Transform& transformOther);
  bool CheckCollision(ColliderSphere colliderSphere, Transform& transform,
                      ColliderCube colliderCubeOther, Transform& transformOther);
  bool CheckCollision(ColliderSphere colliderSphere, Transform& transform,
                      ColliderPolygon colliderPolygonOther, Transform& transformOther);

  // Polygon : { Rest of the colliders }
  bool CheckCollision(ColliderPolygon colliderPolygon, Transform& transform,
                      ColliderPolygon colliderPolygonOther, Transform& transformOther);
  bool CheckCollision(ColliderPolygon colliderPolygon, Transform& transform,
                      ColliderCube colliderCubeOther, Transform& transformOther);
  bool CheckCollision(ColliderPolygon colliderPolygon, Transform& transform,
                      ColliderSphere colliderSphereOther, Transform& transformOther);

  glm::mat4 ApplyTransformation(Transform& transform);
  bool CheckCollisionBetweenBoundingBox(BoundingBox boundingBox, BoundingBox boundingBoxOther);
};