#pragma once

#include "constants.h"
#include "shader.h"
#include "glm.hpp"
#include "boundingBox.h"

// Serialization
#include <sstream>
#include <cereal/archives/json.hpp>

/*
   A common place to store components.
   Components should just be defined as POD (Plain Old Data)
   i.e. simply a struct.

  Note : the structs/components that are declared here should have a default
   value.
*/

struct Transform
{
  glm::vec3 position = glm::vec3(0.0f);
  glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(1.0f);

  template <class Archive>
  void serialize(Archive& archive)
  {
    archive(
        // position
        cereal::make_nvp("position.x", position.x),  //
        cereal::make_nvp("position.y", position.y),  //
        cereal::make_nvp("position.z", position.z),  //
        // rotation
        cereal::make_nvp("rotation.w", rotation.w),  //
        cereal::make_nvp("rotation.x", rotation.x),  //
        cereal::make_nvp("rotation.y", rotation.y),  //
        cereal::make_nvp("rotation.z", rotation.z),  //
        // scale
        cereal::make_nvp("scale.x", scale.x),  //
        cereal::make_nvp("scale.y", scale.y),  //
        cereal::make_nvp("scale.z", scale.z)   //
    );
  }
};

// For now, RigidBody will also act as a collider
// as it is simpler this way
struct RigidBody
{
  bool applyGravity = true;
  glm::vec3 velocity = glm::vec3(0.0f);
};

struct Animation
{
  float animCounter = 0;   // Counter on time passed
  float animInterval = 1;  // Counter on the threshold of each animation

  int spriteCurrent = 0;  // Current sprite
  int spriteStart = 0;
  int spriteEnd = 0;
};

struct SoundEffect
{
  const char* filePath;
  bool looping = false;
  int gain = 1;
};

struct Music
{
  const char* filePath;
  int gain = 1;
};
