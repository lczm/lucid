#pragma once

#include <limits>
#include "glm.hpp"
#include "boundingBox.h"

class ColliderCube
{
 public:
  bool collided = false;
  glm::vec3 color = {1.0f, 1.0f, 1.0f};

 public:
  ColliderCube();
  ~ColliderCube();
};

class ColliderSphere
{
 public:
  bool collided = false;
  glm::vec3 color = {1.0f, 1.0f, 1.0f};

 public:
  ColliderSphere();
  ~ColliderSphere();
};

class ColliderPolygon
{
 public:
  bool collided = false;
  glm::vec3 color = {1.0f, 1.0f, 1.0f};

 public:
  ColliderPolygon();
  ~ColliderPolygon();
};