#pragma once

#include <vector>

#include "gl.h"
#include "glm.hpp"

class Sphere
{
 public:
  // Default color to grey
  glm::vec3 color = {0.5f, 0.5f, 0.5f};

 public:
  Sphere();
  ~Sphere();
};