#pragma once

#include <vector>

#include "gl.h"
#include "glm.hpp"

struct CubeVertex {
  glm::vec3 position;
};

const std::vector<float> cube_vertices = {
    // front
    -1.0, -1.0, 1.0, 0.5, 0.5, 0.5,  //
    1.0, -1.0, 1.0, 0.5, 0.5, 0.5,   //
    1.0, 1.0, 1.0, 0.5, 0.5, 0.5,    //
    -1.0, 1.0, 1.0, 0.5, 0.5, 0.5,   //
    // back
    -1.0, -1.0, -1.0, 0.5, 0.5, 0.5,  //
    1.0, -1.0, -1.0, 0.5, 0.5, 0.5,   //
    1.0, 1.0, -1.0, 0.5, 0.5, 0.5,    //
    -1.0, 1.0, -1.0, 0.5, 0.5, 0.5,   //
};

const std::vector<uint32_t> cube_indices = {
    // front
    0, 1, 2,  //
    2, 3, 0,  //
    // right
    1, 5, 6,  //
    6, 2, 1,  //
    // back
    7, 6, 5,  //
    5, 4, 7,  //
    // left
    4, 0, 3,  //
    3, 7, 4,  //
    // bottom
    4, 5, 1,  //
    1, 0, 4,  //
    // top
    3, 2, 6,  //
    6, 7, 3,  //
};

class Cube {
 public:
  uint32_t VAO, VBO, EBO;
  std::vector<CubeVertex> vertices;
  std::vector<uint32_t> indices;

 public:
  Cube();
  ~Cube();
};