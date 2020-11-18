#pragma once

#include "constants.h"
#include "gl.h"
#include "glm.hpp"

/*
   A standard place to store components.
   Components should just be defined as POD (Plain Old Data)
   i.e. simply a struct.

  Note : the structs/components that are declared here should have a default
   value.
*/

struct Transform {
  glm::vec3 position = glm::vec3(0.0f);
  glm::vec3 rotation = glm::vec3(0.0f);
  glm::vec3 scale = glm::vec3(1.0f);
};

struct RigidBody {
  glm::vec3 velocity = glm::vec3(0.0f);
};

struct Animation {
  float animCounter = 0;   // Counter on time passed
  float animInterval = 1;  // Counter on the threshold of each animation

  int spriteCurrent = 0;  // Current sprite
  int spriteStart = 0;
  int spriteEnd = 0;
};
