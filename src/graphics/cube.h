#pragma once

#include <vector>

#include "shader.h"
#include "glm.hpp"
#include "cereal/archives/json.hpp"

const std::vector<float> cubeVertices = {
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

const std::vector<uint32_t> cubeIndices = {
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

class Cube
{
 public:
  // Default color to grey
  glm::vec3 color = {0.5f, 0.5f, 0.5f};

 public:
  Cube();
  ~Cube();

  template <class Archive>
  void serialize(Archive& archive)
  {
    archive(                                   //
        cereal::make_nvp("color.r", color.r),  //
        cereal::make_nvp("color.g", color.g),  //
        cereal::make_nvp("color.b", color.b)   //
    );
  }
};