#include "camera.h"

Camera::Camera()
{
  const float fov = 45.0f;
  const float near = 0.1f;
  const float far = 250.0f;

  projection = glm::perspective(
      glm::radians(fov), static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), near,
      far);
}

Camera::~Camera() = default;

glm::mat4 Camera::GetProjection()
{
  return projection;
}
