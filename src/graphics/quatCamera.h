#pragma once

#include <iostream>

#include "constants.h"
#include "glm.hpp"
#include "gtx/string_cast.hpp"
#include "gtc/matrix_transform.hpp"

class QuatCamera {
 public:
  glm::vec3 position = glm::vec3(0.0f, 0.0f, -5.0f);
  glm::quat orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  glm::mat4 projection;

  float lastX = 0;
  float lastY = 0;
  float yaw = 0;
  float pitch = 0;
  float roll = 0;

  float twoPi = glm::two_pi<float>();

 public:
  QuatCamera();
  ~QuatCamera();

  glm::vec3& GetPosition();
  glm::quat& GetOrientation();
  glm::mat4& GetProjection();
  glm::mat4& GetView();

  void Translate(const glm::vec3& vec);
  void Translate(const float x, const float y, const float z);

  void Rotate(const float angle, const glm::vec3& axis);
  void Rotate(const float angle, const float x, const float y, const float z);

  void Pitch(const float angle);  // x-axis
  void Yaw(const float angle);    // y-axis
  void Roll(const float angle);   // z-axis

  void Update(double dt, float offsetX, float offsetY);
};