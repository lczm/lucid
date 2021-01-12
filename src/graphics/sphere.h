#pragma once

#include <vector>

#include "shader.h"
#include "glm.hpp"

class Sphere
{
 public:
  // Default color to grey
  glm::vec3 color = {0.5f, 0.5f, 0.5f};

 public:
  Sphere();
  ~Sphere();

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