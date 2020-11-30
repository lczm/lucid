#pragma once

#include "constants.h"
#include "gl.h"
#include "glm.hpp"

#include "boundingBox.h"

/*
   A common place to store components.
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

// For now, RigidBody will also act as a collider
// as it is simpler this way
struct RigidBody {
  glm::vec3 velocity = glm::vec3(0.0f);
};

enum class ColliderType {
  Cube,
  Sphere,
  Model,
  None,
};

struct Collider {
  ColliderType type = ColliderType::Sphere;
  BoundingBox oobb;  // Oriented Bounding Box
};

struct ColliderSphere {
  // There is no need for a center as that is the under position...?
  uint32_t radius = 5;  // Default...?
};

struct ColliderCube {};

struct ColliderModel {};

struct Animation {
  float animCounter = 0;   // Counter on time passed
  float animInterval = 1;  // Counter on the threshold of each animation

  int spriteCurrent = 0;  // Current sprite
  int spriteStart = 0;
  int spriteEnd = 0;
};

struct SoundEffect {
  const char* filePath;
  bool looping = false;
  int gain = 1;
};

struct Music {
  const char* filePath;
  int gain = 1;
};
