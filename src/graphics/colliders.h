#pragma once

#include <limits>
#include "glm.hpp"
#include "boundingBox.h"

class ColliderCube
{
 public:
  uint32_t VAO, VBO, EBO;

  bool collided = false;
  glm::vec3 color = {1.0f, 1.0f, 1.0f};

 public:
  ColliderCube();
  ~ColliderCube();

  void Build();
};

class ColliderSphere
{
 public:
  uint32_t VAO, VBO, EBO;

  bool collided = false;
  glm::vec3 color = {1.0f, 1.0f, 1.0f};

 public:
  ColliderSphere();
  ~ColliderSphere();
};

class ColliderPolygon
{
 public:
  uint32_t VAO, VBO, EBO;

  bool collided = false;
  glm::vec3 color = {1.0f, 1.0f, 1.0f};

 public:
  ColliderPolygon();
  ~ColliderPolygon();
};