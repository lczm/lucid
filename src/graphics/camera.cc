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

glm::vec3 Camera::GetPosition()
{
  return position;
}

glm::vec3 Camera::GetPositionInWorld()
{
  return {-position.x, -position.y, -position.z};
}

glm::quat Camera::GetOrientation()
{
  return orientation;
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return glm::translate(glm::mat4_cast(orientation), position);
}

void Camera::Translate(const glm::vec3 vec)
{
  position += vec * orientation;
}

void Camera::TranslateInWorld(const glm::vec3 vec)
{
  position += -vec * orientation;
}

void Camera::Rotate(const float angle, const glm::vec3 axis)
{
  orientation *= glm::angleAxis(angle, axis * orientation);
}

void Camera::Rotate(const float angle, const float x, const float y, const float z)
{
  orientation *= glm::angleAxis(angle, glm::vec3(x, y, z) * orientation);
}

// x-axis
void Camera::Pitch(const float angle)
{
  Rotate(angle, 1.0f, 0.0f, 0.0f);
}

// y-axis
void Camera::Yaw(const float angle)
{
  Rotate(angle, 0.0f, 1.0f, 0.0f);
}

// z-axis
void Camera::Roll(const float angle)
{
  Rotate(angle, 0.0f, 0.0f, 1.0f);
}

void Camera::PanCamera(float dt, float offsetX, float offsetY)
{
  const float damp = 0.00005f;

  offsetX *= CAMERA_SENSITIVITY * dt;
  // This is always negative so that 'moving' the camera downwards feels natural
  // As when it is 'right -clicked', it is in the opposite direction
  offsetY *= -(CAMERA_SENSITIVITY * dt);

  yaw += offsetX;
  pitch += offsetY;

  if (pitch > twoPi)
    pitch -= twoPi;
  else if (pitch < -twoPi)
    pitch += twoPi;

  if (yaw > twoPi)
    yaw -= twoPi;
  else if (yaw < -twoPi)
    yaw += twoPi;

  if (pitch != 0.0f) Pitch(pitch);
  if (yaw != 0.0f) Yaw(yaw);
  if (roll != 0.0f) Roll(roll);

  pitch *= damp;
  yaw *= damp;
  roll *= damp;
}