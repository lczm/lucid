#pragma once

#include <iostream>

#include "constants.h"
#include "component.h"

#include "glm.hpp"
#include "gtx/string_cast.hpp"
#include "gtc/matrix_transform.hpp"
#include "cereal/archives/json.hpp"

class Camera
{
 public:
  glm::mat4 projection = glm::mat4(1.0f);

  float lastX = 0;
  float lastY = 0;

  float yaw = 0;
  float pitch = 0;
  float roll = 0;

 public:
  Camera();
  ~Camera();

  glm::mat4 GetProjection();

  template <class Archive>
  void serialize(Archive& archive)
  {
    archive(
        // projection
        cereal::make_nvp("projection.col0.x", projection[0].x),  //
        cereal::make_nvp("projection.col0.y", projection[0].y),  //
        cereal::make_nvp("projection.col0.z", projection[0].z),  //
        cereal::make_nvp("projection.col0.w", projection[0].w),  //

        cereal::make_nvp("projection.col1.x", projection[1].x),  //
        cereal::make_nvp("projection.col1.y", projection[1].y),  //
        cereal::make_nvp("projection.col1.z", projection[1].z),  //
        cereal::make_nvp("projection.col1.w", projection[1].w),  //

        cereal::make_nvp("projection.col2.x", projection[2].x),  //
        cereal::make_nvp("projection.col2.y", projection[2].y),  //
        cereal::make_nvp("projection.col2.z", projection[2].z),  //
        cereal::make_nvp("projection.col2.w", projection[2].w),  //

        cereal::make_nvp("projection.col3.x", projection[3].x),  //
        cereal::make_nvp("projection.col3.y", projection[3].y),  //
        cereal::make_nvp("projection.col3.z", projection[3].z),  //
        cereal::make_nvp("projection.col3.w", projection[3].w),  //

        // lastX
        cereal::make_nvp("lastX", lastX),  //
        // lastY
        cereal::make_nvp("lastY", lastY),  //
        // yaw
        cereal::make_nvp("yaw", yaw),  //
        // pitch
        cereal::make_nvp("pitch", pitch),  //
        // roll
        cereal::make_nvp("roll", roll)  //
    );
  }
};