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

 public:
  Line();
  ~Line();
};