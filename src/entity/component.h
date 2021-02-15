#pragma once

#include "constants.h"
#include "shader.h"
#include "glm.hpp"
#include "boundingBox.h"

// Serialization
#include <sstream>
#include "cereal/archives/json.hpp"

/*
   A common place to store components.
   Components should just be defined as POD (Plain Old Data)
   i.e. simply a struct.

  Note : the structs/components that are declared here should have a default
   value.
*/

struct Adder
{
};

struct Deleter
{
};

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

  template <class Archive>
  void serialize(Archive& archive)
  {
    archive(
        // applyGravity
        cereal::make_nvp("applyGravity", applyGravity),  //
        // velocity
        cereal::make_nvp("velocity.x", velocity.x),  //
        cereal::make_nvp("velocity.y", velocity.y),  //
        cereal::make_nvp("velocity.z", velocity.z)   //
    );
  }
};
