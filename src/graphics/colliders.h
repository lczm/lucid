#pragma once

#include <limits>
#include "glm.hpp"
#include "boundingBox.h"
#include "component.h"
#include "engineComponents.h"

class ColliderCube
{
 public:
  Transform transform;
  bool collided = false;
  glm::vec3 color = {1.0f, 1.0f, 1.0f};

 public:
  ColliderCube();
  ~ColliderCube();

  glm::vec3 Support(Transform& transform, glm::vec3 direction);
};

class ColliderSphere
{
 public:
  Transform transform;
  bool collided = false;
  glm::vec3 color = {1.0f, 1.0f, 1.0f};

 public:
  ColliderSphere();
  ~ColliderSphere();

  glm::vec3 Support(Transform& transform, glm::vec3 direction);
};

class ColliderPolygon
{
 public:
  Transform transform;
  std::vector<glm::vec4> vertices;

  bool collided = false;
  glm::vec3 color = {1.0f, 1.0f, 1.0f};

 public:
  ColliderPolygon();
  ~ColliderPolygon();

  // Use case : when the user creates an entity <Model, ColliderPolygon>
  // the user will also call colliderPolygon.SetVertices(model.GetVertices());
  // The reason behind this is so that the user has more flexibility when creating
  // collider polygons for the shapes.
  void SetVertices(std::vector<glm::vec4> vertices);
  glm::vec3 Support(Transform& transform, glm::vec3 direction);
};