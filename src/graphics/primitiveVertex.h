#pragma once

#include <vector>
#include "glm.hpp"

/*
 * Although all the primitive vertices are essentially the same thing,
 * I chose to have them be separated because this can be extended in the future
 * in the case a specific primtiive has a new specific need.
 */

struct LineVertex
{
  glm::vec3 color;
  glm::mat4 modelMatrix;
};

struct SphereVertex
{
  glm::vec3 color;
  glm::mat4 modelMatrix;
};

struct CubeVertex
{
  glm::vec3 color;
  glm::mat4 modelMatrix;
};