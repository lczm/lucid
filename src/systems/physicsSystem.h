#pragma once

#include "shader.h"
#include "ecs.h"
#include "shapes.h"
#include "model.h"
#include "colliders.h"
#include "engineComponents.h"
#include "gtx/string_cast.hpp"

enum class Intersection
{
  Unknown,
  None,
  Found,
};

class PhysicsSystem : public System
{
 public:
  PhysicsSystem();
  ~PhysicsSystem();

  void Update(float dt, Registry* registry, Input* input);

  void UpdateAllRigidbodies(float dt, Registry* registry, Input* input);
  void UpdateCollisions(float dt, Registry* registry, Input* input);

  bool CheckCollision(Collider& colliderA, Transform transformA, Collider& colliderB,
                      Transform transformB);

  Intersection EvolveSimplex(std::vector<glm::vec3>& vertices, glm::vec3 direction,
                             Collider& colliderA, Transform transformA, Collider& colliderB,
                             Transform transformB);

  bool AddSupport(std::vector<glm::vec3>& vertices, glm::vec3 direction, Collider& colliderA,
                  Transform transformA, Collider& colliderB, Transform transformB);

  glm::vec3 CalculateSupport(glm::vec3 direction, Collider& colliderA, const Transform transformA,
                             Collider& colliderB, const Transform transformB);

  bool CheckCollisionBetweenBoundingBox(BoundingBox boundingBox, BoundingBox boundingBoxOther);
};