#pragma once

#include "glm.hpp"

/*
 * This is mainly structs used to pass data around in the various systems.
 * These structs should not be included in the release builds but rather only for the debug builds
 * e.g. when the renderSystem needs to know if a certain checkbox is clicked from the uisystem, this
 * is where the structs are stored.
 */

struct DevDebug {
  bool drawWireframe = false;

  glm::vec3 rgb = glm::vec3(0.5f, 0.5f, 0.5f);

  glm::vec3 cameraUp = glm::vec3(1.0f);
  glm::vec3 cameraFront = glm::vec3(1.0f);
  glm::vec3 cameraPos = glm::vec3(1.0f);
};