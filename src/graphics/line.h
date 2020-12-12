#pragma once

#include <vector>

#include "gl.h"
#include "glm.hpp"

const std::vector<float> line_vertices = {
    0.0f, 0.0f, 0.0f,  //
    1.0f, 1.0f, 1.0f,  //
};

class Line
{
 public:
  glm::vec3 origin = glm::vec3(0.0f);
  glm::vec3 destination = glm::vec3(0.0f);

  // Default color to grey
  glm::vec3 color = {0.5f, 0.5f, 0.5f};

 public:
  Line();
  ~Line();
};
