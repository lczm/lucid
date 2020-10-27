#include "camera.h"

Camera::Camera() {
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
}

Camera::~Camera() {
}

glm::mat4 Camera::GetView() {
  return glm::mat4(1.0f);
}

glm::mat4 Camera::GetProjection() {
  return glm::perspective(glm::radians(fov),
                          static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT),
                          near, far);
}

glm::vec3 Camera::GetCameraPos() {
  return glm::vec3(1.0f);
}