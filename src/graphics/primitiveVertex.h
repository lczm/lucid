#pragma once

#include "glm.hpp"

struct LineVertex
{
  glm::vec3 color;
  glm::mat4 modelMatrix;
};

struct SphereVertex
{
};

struct CubeVertex
{
  glm::vec3 color;
  glm::mat4 modelMatrix;
};