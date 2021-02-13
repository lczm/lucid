#pragma once

#include <iostream>

#include "constants.h"
#include "component.h"

#include "glm.hpp"
#include "gtx/string_cast.hpp"
#include "gtc/matrix_transform.hpp"

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
};