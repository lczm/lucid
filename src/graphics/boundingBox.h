#pragma once

#include <limits>
#include "glm.hpp"
#include "gl.h"
#include "glm.hpp"

struct BoundingBox {
  float minX = std::numeric_limits<float>::max();
  float maxX = std::numeric_limits<float>::lowest();

  float minY = std::numeric_limits<float>::max();
  float maxY = std::numeric_limits<float>::lowest();

  float minZ = std::numeric_limits<float>::max();
  float maxZ = std::numeric_limits<float>::lowest();
};

const std::vector<float> boundingBoxCubeVertices = {
    // front
    -1.0, -1.0, 1.0,  //
    1.0, -1.0, 1.0,   //
    1.0, 1.0, 1.0,    //
    -1.0, 1.0, 1.0,   //
    // back
    -1.0, -1.0, -1.0,  //
    1.0, -1.0, -1.0,   //
    1.0, 1.0, -1.0,    //
    -1.0, 1.0, -1.0,   //
};

const std::vector<uint32_t> boundingBoxCubeIndices = {
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

class BoundingBoxCube {
 public:
  uint32_t VAO, VBO, EBO;

  glm::vec3 color = {1.0f, 1.0f, 1.0f};

 public:
  BoundingBoxCube();
  ~BoundingBoxCube();
};