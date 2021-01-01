#pragma once

#include "shader.h"
#include "ecs.h"
#include "cube.h"
#include "sphere.h"
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
  Intersection EvolveSimplex(Transform transformA, Transform transformB);

  glm::vec3 CalculateSupport(glm::vec3 direction);
  bool AddSupport(glm::vec3 direction);

  glm::mat4 ApplyTransformation(Transform& transform);
  bool CheckCollisionBetweenBoundingBox(BoundingBox boundingBox, BoundingBox boundingBoxOther);
};