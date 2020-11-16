#pragma once

#include <vector>

#include "gl.h"
#include "glm.hpp"

class Sphere {
 public:
  uint32_t VAO, VBO, EBO;

  std::vector<float> vertices;
  std::vector<float> normals;
  std::vector<float> texCoords;

  std::vector<uint32_t> indices;
  std::vector<float> interleavedVertices;

  float radius;
  int sectors;
  int stacks;

 public:
  Sphere();
  Sphere(float radius, int sectors, int stacks);
  ~Sphere();

  void Build();
  void BuildSphere();
};