#pragma once

#include <vector>

#include "gl.h"
#include "glm.hpp"

const std::vector<float> line_vertices = {
    -1.0,
    1.0,
};

class Line {
 public:
  // Line does not need index buffer
  uint32_t VAO, VBO;

  // Default color to grey
  glm::vec3 color = {0.5f, 0.5f, 0.5f};

 public:
  Line();
  ~Line();
};