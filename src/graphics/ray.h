#pragma once

#include "ecs.h"
#include "input.h"
#include "glm.hpp"

static glm::vec3 GetRayDirection(Registry* registry, Input* input)
{
#if DEBUG
  WidgetLayout& widgetLayout = registry->GetEditorResource<WidgetLayout>();
  float mouseX = static_cast<float>(input->GetMouseX() - widgetLayout.leftWindowWidth);
  float mouseY = static_cast<float>(input->GetMouseYAbsolute() - widgetLayout.menuBarHeight -
                                    widgetLayout.topWindowHeight);

  // lucid::Log("x : ", input->GetMouseX(), " y : ", input->GetMouseYAbsolute())

  float x = (2.0f * mouseX) /
                (SCREEN_WIDTH - widgetLayout.leftWindowWidth - widgetLayout.rightWindowWidth) -
            1.0f;
  float y = 1.0f - (2.0f * mouseY) / (SCREEN_HEIGHT - widgetLayout.bottomWindowHeight -
                                      widgetLayout.menuBarHeight - widgetLayout.topWindowHeight);
  float z = 1.0f;
#endif

#if RELEASE
  float mouseX = static_cast<float>(input->GetMouseX());
  float mouseY = static_cast<float>(input->GetMouseYAbsolute());

  float x = (2.0f * mouseX) / SCREEN_WIDTH - 1.0f;
  float y = 1.0f - (2.0f * mouseY) / SCREEN_HEIGHT;
  float z = 1.0f;
#endif

  // normalized device coordinates
  glm::vec3 rayNds = glm::vec3(x, y, z);

  // homogeneous clip coordinates
  glm::vec4 rayClip = glm::vec4(rayNds.x, rayNds.y, -1.0f, 1.0f);

  Camera& camera = GetActiveCameraRef(registry);

  // convert to eye/camera coordinates
  glm::vec4 rayEye = glm::inverse(camera.GetProjection()) * rayClip;

  // unproject the x, z part
  rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

  // 4d world coordinates
  // normalize the vector as well
  glm::vec3 rayWorld = glm::vec3(glm::normalize(glm::inverse(camera.GetView()) * rayEye));

  // Note : Do this for visibility, to visualize the ray,
  // the scale of the ray does not affect what it is used for.
  // Scale this by a fairly huge amount
  // rayWorld *= 1000.0f;

  return rayWorld;
}
