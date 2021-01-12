#pragma once

#include <limits>
#include "glm.hpp"
#include "boundingBox.h"
#include "component.h"
#include "engineComponents.h"
#include "renderUtils.h"

class Collider
{
 public:
  virtual glm::vec3 Support(Transform& transform, glm::vec3 direction) = 0;
};

class ColliderCube : public Collider
{
 public:
  Transform transform;
  std::vector<float> vertices;

  bool collided = false;
  glm::vec3 color = {0.0f, 0.0f, 0.0f};

 public:
  ColliderCube();
  ~ColliderCube();

  void SetVertices(std::vector<float> vertices);
  glm::vec3 Support(Transform& transform, glm::vec3 direction);

  template <class Archive>
  void serialize(Archive& archive)
  {
    archive(
        // position
        cereal::make_nvp("transform.position.x", transform.position.x),  //
        cereal::make_nvp("transform.position.y", transform.position.y),  //
        cereal::make_nvp("transform.position.z", transform.position.z),  //
        // rotation
        cereal::make_nvp("transform.rotation.w", transform.rotation.w),  //
        cereal::make_nvp("transform.rotation.x", transform.rotation.x),  //
        cereal::make_nvp("transform.rotation.y", transform.rotation.y),  //
        cereal::make_nvp("transform.rotation.z", transform.rotation.z),  //
        // scale
        cereal::make_nvp("transform.scale.x", transform.scale.x),  //
        cereal::make_nvp("transform.scale.y", transform.scale.y),  //
        cereal::make_nvp("transform.scale.z", transform.scale.z),  //

        cereal::make_nvp("color.r", color.r),  //
        cereal::make_nvp("color.g", color.g),  //
        cereal::make_nvp("color.b", color.b)   //
    );
  }
};

class ColliderSphere : public Collider
{
 public:
  Transform transform;

  bool collided = false;
  glm::vec3 color = {0.0f, 0.0f, 0.0f};

 public:
  ColliderSphere();
  ~ColliderSphere();

  glm::vec3 Support(Transform& transform, glm::vec3 direction);

  template <class Archive>
  void serialize(Archive& archive)
  {
    archive(
        // position
        cereal::make_nvp("transform.position.x", transform.position.x),  //
        cereal::make_nvp("transform.position.y", transform.position.y),  //
        cereal::make_nvp("transform.position.z", transform.position.z),  //
        // rotation
        cereal::make_nvp("transform.rotation.w", transform.rotation.w),  //
        cereal::make_nvp("transform.rotation.x", transform.rotation.x),  //
        cereal::make_nvp("transform.rotation.y", transform.rotation.y),  //
        cereal::make_nvp("transform.rotation.z", transform.rotation.z),  //
        // scale
        cereal::make_nvp("transform.scale.x", transform.scale.x),  //
        cereal::make_nvp("transform.scale.y", transform.scale.y),  //
        cereal::make_nvp("transform.scale.z", transform.scale.z),  //

        cereal::make_nvp("color.r", color.r),  //
        cereal::make_nvp("color.g", color.g),  //
        cereal::make_nvp("color.b", color.b)   //
    );
  }
};

class ColliderPolygon : public Collider
{
 public:
  Transform transform;
  std::vector<glm::vec4> vertices;

  bool collided = false;
  glm::vec3 color = {0.0f, 0.0f, 0.0f};

 public:
  ColliderPolygon();
  ~ColliderPolygon();

  // Use case : when the user creates an entity <Model, ColliderPolygon>
  // the user will also call colliderPolygon.SetVertices(model.GetVertices());
  // The reason behind this is so that the user has more flexibility when creating
  // collider polygons for the shapes.
  void SetVertices(std::vector<glm::vec4> vertices);
  glm::vec3 Support(Transform& transform, glm::vec3 direction);

  template <class Archive>
  void serialize(Archive& archive)
  {
    archive(
        // position
        cereal::make_nvp("transform.position.x", transform.position.x),  //
        cereal::make_nvp("transform.position.y", transform.position.y),  //
        cereal::make_nvp("transform.position.z", transform.position.z),  //
        // rotation
        cereal::make_nvp("transform.rotation.w", transform.rotation.w),  //
        cereal::make_nvp("transform.rotation.x", transform.rotation.x),  //
        cereal::make_nvp("transform.rotation.y", transform.rotation.y),  //
        cereal::make_nvp("transform.rotation.z", transform.rotation.z),  //
        // scale
        cereal::make_nvp("transform.scale.x", transform.scale.x),  //
        cereal::make_nvp("transform.scale.y", transform.scale.y),  //
        cereal::make_nvp("transform.scale.z", transform.scale.z),  //

        cereal::make_nvp("color.r", color.r),  //
        cereal::make_nvp("color.g", color.g),  //
        cereal::make_nvp("color.b", color.b)   //
    );
  }
};