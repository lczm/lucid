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
  // For the position and orientation/rotation
  // Scale is not used here
  Transform transform;

  glm::mat4 projection = glm::mat4(1.0f);

  float lastX = 0;
  float lastY = 0;

  float yaw = 0;
  float pitch = 0;
  float roll = 0;

  float twoPi = glm::two_pi<float>();

 public:
  Camera();
  ~Camera();

  glm::vec3 GetPosition();
  glm::vec3 GetPositionInWorld();

  glm::quat GetRotation();
  glm::mat4 GetProjection();
  glm::mat4 GetView();

  void Translate(const glm::vec3 vec);
  void Translate(const float x, const float y, const float z);
  void TranslateInWorld(const glm::vec3 vec);

  void Rotate(const float angle, const glm::vec3 axis);
  void Rotate(const float angle, const float x, const float y, const float z);

  void Pitch(const float angle);  // x-axis
  void Yaw(const float angle);    // y-axis
  void Roll(const float angle);   // z-axis

  void PanCamera(float dt, float offsetX, float offsetY);
};