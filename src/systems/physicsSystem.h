#pragma once

#include "shader.h"
#include "ecs.h"
#include "cube.h"
#include "sphere.h"
#include "model.h"
#include "colliders.h"
#include "engineComponents.h"
#include "gtx/string_cast.hpp"

enum class EvolveResult
{
  NoIntersection,
  FoundIntersection,
  StillEvolving,
};

class PhysicsSystem : public System
{
 private:
  std::vector<glm::vec3> vertices;
  glm::vec3 direction;
  Collider* colliderA;
  Collider* colliderB;
  Transform* transformA;
  Transform* transformB;

 public:
  PhysicsSystem();
  ~PhysicsSystem();

  void Update(float dt, Registry* registry, Input* input);

  void UpdateAllRigidbodies(float dt, Registry* registry, Input* input);
  void UpdateCollisions(float dt, Registry* registry, Input* input);

  void Test(float dt, Registry* registry, Input* input);
  EvolveResult EvolveSimplex(Transform transformA, Transform transformB);

  glm::vec3 CalculateSupport(glm::vec3 direction);
  bool AddSupport(glm::vec3 direction);

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