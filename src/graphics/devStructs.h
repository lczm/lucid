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
  bool drawColliders = false;

  // fps is updated per frame
  uint32_t fps = 0;
  uint32_t rayID = 0;

  float leftWindowWidth = 0;
  float leftWindowHeight = 0;

  float rightWindowWidth = 0;
  float rightWindowHeight = 0;
};