#include "camera.h"

Camera::Camera()
{
  yaw = 0.0f;
  pitch = 0.0f;
  lastX = 0;
  lastY = 0;

  fov = 45.0f;
  near = 0.1f;
  far = 100.0f;

  // The camera's up vector is pointing towards the y-axis
  cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
  cameraFront =
      glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)),
                sin(glm::radians(yaw)) * cos(glm::radians(pitch)));

  cameraPos = glm::normalize(glm::cross(cameraFront, cameraUp));
  cameraUp = glm::normalize(glm::cross(cameraPos, cameraFront));

  projection = glm::perspective(
      glm::radians(fov), static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), near,
      far);
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetView()
{
  return view;
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::vec3 Camera::GetCameraPos()
{
  return cameraPos;
}

void Camera::UpdateView()
{
  view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::UpdateCameraVector(float xOffset, float yOffset)
{
  xOffset *= CAMERA_SENSITIVITY;
  yOffset *= CAMERA_SENSITIVITY;

  yaw += xOffset;
  pitch += yOffset;

  if (pitch > 89.0f) pitch = 89.0f;
  if (pitch < -89.0f) pitch = -89.0f;

  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  cameraFront = glm::normalize(front);

  // Note: not too sure if this is needed... update camera to a free camera
  // before changing
  glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
  cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

void Camera::MoveCamera(glm::vec3 offset)
{
  cameraPos += offset;
}