#pragma once

#include "constants.h"
#include "glm.hpp"
#include "gtx/string_cast.hpp"
#include "gtc/matrix_transform.hpp"

class Camera {
 public:
  glm::vec3 cameraPos;
  glm::vec3 cameraUp;
  glm::vec3 cameraFront;

  glm::mat4 view;
  glm::mat4 projection;

  float fov;
  float near;
  float far;

  float lastX, lastY;
  float yaw, pitch;

 public:
  Camera();
  ~Camera();

  glm::mat4 GetView();
  glm::mat4 GetProjection();
  glm::vec3 GetCameraPos();

  void UpdateView();
  void UpdateCameraVector(float xOffset, float yOffset);

  void MoveCamera(glm::vec3 offset);
};