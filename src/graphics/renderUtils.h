#pragma once

#include <iostream>

#include "ecs.h"
#include "input.h"
#include "component.h"
#include "camera.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/quaternion.hpp"
#include "gtc/type_ptr.hpp"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

enum class DrawType
{
  Line,    //
  Sphere,  //
  Cube,    //
  Model,   //
};

/*
 * TODO : Update this to the quaternion based representation
 * this should be updated when transform.rotation is updated to become a glm::quat
 */
// static glm::mat4 GetModelMatrix(const Transform transform)
// {
//   glm::mat4 matrixModel = glm::mat4(1.0f);
//   glm::mat4 rotationMatrix = glm::mat4(1.0f);
//
//   matrixModel = glm::translate(matrixModel, transform.position);
//   matrixModel = glm::scale(matrixModel, transform.scale);
//
//   // Rotation matrix
//   rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[0], glm::vec3(1.0, 0.0, 0.0));
//   rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[1], glm::vec3(0.0, 1.0, 0.0));
//   rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[2], glm::vec3(0.0, 0.0, 1.0));
//
//   matrixModel *= rotationMatrix;
//
//   return matrixModel;
// }

static glm::mat4 GetModelMatrix(const Transform transform)
{
  glm::mat4 matrixModel = glm::mat4(1.0f);

  glm::mat4 rotationMatrix = glm::mat4_cast(transform.rotation);
  // glm::mat4 rotationMatrix = glm::mat4(1.0f);

  matrixModel = glm::translate(matrixModel, transform.position);
  matrixModel = glm::scale(matrixModel, transform.scale);

  // Rotation matrix
  // rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[0], glm::vec3(1.0, 0.0, 0.0));
  // rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[1], glm::vec3(0.0, 1.0, 0.0));
  // rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[2], glm::vec3(0.0, 0.0, 1.0));

  matrixModel *= rotationMatrix;

  return matrixModel;
}

// static glm::mat4 GetModelMatrix(glm::mat4 matrixModel, const Transform transform)
// {
//   glm::mat4 rotationMatrix = glm::mat4(1.0f);
//
//   matrixModel = glm::translate(matrixModel, transform.position);
//   matrixModel = glm::scale(matrixModel, transform.scale);
//
//   // Rotation matrix
//   rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[0], glm::vec3(1.0, 0.0, 0.0));
//   rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[1], glm::vec3(0.0, 1.0, 0.0));
//   rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[2], glm::vec3(0.0, 0.0, 1.0));
//
//   matrixModel *= rotationMatrix;
//
//   return matrixModel;
// }

static glm::mat4 GetModelMatrix(glm::mat4 matrixModel, const Transform transform)
{
  glm::mat4 rotationMatrix = glm::toMat4(transform.rotation);

  matrixModel = glm::translate(matrixModel, transform.position);
  matrixModel = glm::scale(matrixModel, transform.scale);
  matrixModel *= rotationMatrix;

  return matrixModel;
}

static glm::mat4 GetModelMatrix(Transform transformA, Transform transformB)
{
  glm::mat4 matrixModel = glm::mat4(1.0f);

  // Transform A
  matrixModel = glm::translate(matrixModel, transformA.position);
  matrixModel = glm::scale(matrixModel, transformA.scale);
  matrixModel *= glm::mat4_cast(transformA.rotation);

  // Transform B
  matrixModel = glm::translate(matrixModel, transformB.position);
  matrixModel = glm::scale(matrixModel, transformB.scale);
  matrixModel *= glm::mat4_cast(transformB.rotation);

  return matrixModel;
}

// This will be useful in the situation in (e.g.) trying to get a gigantic
// axis aligned bounding box, without the rotations
static glm::mat4 GetModelMatrixWithoutRotation(const Transform transform)
{
  glm::mat4 matrixModel = glm::mat4(1.0f);

  matrixModel = glm::translate(matrixModel, transform.position);
  matrixModel = glm::scale(matrixModel, transform.scale);

  return matrixModel;
}

/*
 * Utility functions to help cast from assimp data structures to the
 * equivalent in glm
 */

static inline glm::mat4 CastToGlmMat4(aiMatrix4x4 from)
{
  return glm::transpose(glm::make_mat4(&from.a1));
}

static inline glm::mat4 CastToGlmMat4(aiMatrix3x3 from)
{
  return glm::transpose(glm::make_mat3(&from.a1));
}

static inline glm::mat4 CastToGlmMat4(aiVector3D from)
{
  return glm::translate(glm::mat4(1.0f), glm::vec3(from.x, from.y, from.z));
}

static inline glm::mat4 CastToGlmMat4Scale(aiVector3D from)
{
  return glm::scale(glm::mat4(1.0f), glm::vec3(from.x, from.y, from.z));
}

static inline glm::mat4 CastToGlmMat4(aiQuaternion from)
{
  return CastToGlmMat4(from.GetMatrix());
}

static inline glm::quat CastToGlmQuat(aiQuaternion from)
{
  return glm::quat(from.w, from.x, from.y, from.z);
}

static inline glm::vec2 CastToVec2(aiVector3D from)
{
  return glm::vec2(from.x, from.y);
}

static inline glm::vec3 CastToVec3(aiVector3D from)
{
  return glm::vec3(from.x, from.y, from.z);
}

static std::vector<glm::vec4> GetCubeVertices(glm::mat4 modelMatrix)
{
  std::vector<glm::vec4> vertices;
  vertices.reserve(boundingBoxCubeVertices.size() / 3);

  for (size_t i = 0; i < boundingBoxCubeVertices.size(); i += 3)
  {
    vertices.push_back(modelMatrix * glm::vec4(boundingBoxCubeVertices[i],
                                               boundingBoxCubeVertices[i + 1],
                                               boundingBoxCubeVertices[i + 2], 1.0f));
  }

  return vertices;
}

static std::vector<glm::vec4> GetCubeVertices(glm::mat4 modelMatrix, std::vector<float> vertices)
{
  std::vector<glm::vec4> newVertices;
  newVertices.reserve(vertices.size() / 3);

  for (size_t i = 0; i < vertices.size(); i += 3)
  {
    newVertices.push_back(modelMatrix *
                          glm::vec4(vertices[i], vertices[i + 1], vertices[i + 2], 1.0f));
  }

  return newVertices;
}

static BoundingBox GetBoundingBox(std::vector<glm::vec4> vertices)
{
  BoundingBox bb;
  for (size_t i = 0; i < vertices.size(); i++)
  {
    bb.minX = glm::min(vertices[i].x, bb.minX);
    bb.maxX = glm::max(vertices[i].x, bb.maxX);

    bb.minY = glm::min(vertices[i].y, bb.minY);
    bb.maxY = glm::max(vertices[i].y, bb.maxY);

    bb.minZ = glm::min(vertices[i].z, bb.minZ);
    bb.maxZ = glm::max(vertices[i].z, bb.maxZ);
  }
  return bb;
}

static std::vector<float> GetBoundingBoxVertices(BoundingBox bb)
{
  return std::vector<float>{
      bb.minX, bb.minY, bb.maxZ, bb.maxX, bb.minY, bb.maxZ,
      bb.maxX, bb.maxY, bb.maxZ, bb.minX, bb.maxY, bb.maxZ,

      bb.minX, bb.minY, bb.minZ, bb.maxX, bb.minY, bb.minZ,
      bb.maxX, bb.maxY, bb.minZ, bb.minX, bb.maxY, bb.minZ,
  };
}

static std::vector<glm::vec4> ConvertFloatToVecVertices(std::vector<float> vertices)
{
  std::vector<glm::vec4> newVertices;
  newVertices.reserve(vertices.size() / 3);

  for (size_t i = 0; i < vertices.size(); i += 3)
  {
    newVertices.push_back(glm::vec4(vertices[i], vertices[i + 1], vertices[i + 2], 1.0f));
  }

  return newVertices;
}

static inline Transform TranslateInWorld(Transform transform, const glm::vec3 vec)
{
  transform.position += vec;
  return transform;
}

/*
 * Quat rotation utilities
 * These are inlined because they should not really require any
 * function call overheads.
 */

// Angles in radians, use glm::radians to convert degree to radians
static inline glm::quat RotateQuat(glm::quat quat, float angle, glm::vec3 axis)
{
  return quat *= glm::angleAxis(angle, axis);
}

// Angles in radians, use glm::radians to convert degree to radians
static inline glm::quat RotateQuatX(glm::quat quat, float angle)
{
  return quat *= glm::angleAxis(angle, glm::vec3(1.0f, 0.0f, 0.0f));
}

// Angles in radians, use glm::radians to convert degree to radians
static inline glm::quat RotateQuatY(glm::quat quat, float angle)
{
  return quat *= glm::angleAxis(angle, glm::vec3(0.0f, 1.0f, 0.0f));
}

// Angles in radians, use glm::radians to convert degree to radians
static inline glm::quat RotateQuatZ(glm::quat quat, float angle)
{
  return quat *= glm::angleAxis(angle, glm::vec3(0.0f, 0.0f, 1.0f));
}

static inline glm::vec3 GetPosition(const Transform transform)
{
  return transform.position;
}

static inline glm::vec3 GetPositionInWorld(const Transform transform)
{
  return -transform.position;
}

static inline glm::quat GetRotation(const Transform transform)
{
  return transform.rotation;
}

static inline glm::vec3 GetScale(const Transform transform)
{
  return transform.scale;
}

static inline glm::mat4 GetProjection(const Camera camera)
{
  return camera.projection;
}

static inline glm::mat4 GetView(const Transform transform)
{
  return glm::translate(glm::mat4_cast(transform.rotation), transform.position);
}

static inline Camera* GetActiveCameraPtr(Registry* registry)
{
  DevDebug& devDebug = registry->GetEditorResource<DevDebug>();

  if (devDebug.activeCamera == CameraType::Scene)
  {
    return &(registry->GetComponent<Camera>(devDebug.sceneCameraId));
  }
  else if (devDebug.activeCamera == CameraType::Game)
  {
    return &(registry->GetComponent<Camera>(devDebug.gameCameraId));
  }
  else
  {
    std::cout << "Attempted to GetActiveCameraPtr a component that does not exist" << std::endl;
    exit(0);
  }
}

static inline Camera& GetActiveCameraRef(Registry* registry)
{
  DevDebug& devDebug = registry->GetEditorResource<DevDebug>();

  if (devDebug.activeCamera == CameraType::Scene)
  {
    return registry->GetComponent<Camera>(devDebug.sceneCameraId);
  }
  else if (devDebug.activeCamera == CameraType::Game)
  {
    return registry->GetComponent<Camera>(devDebug.gameCameraId);
  }
  else
  {
    std::cout << "Attempted to GetActiveCameraRef a component that does not exist" << std::endl;
    exit(0);
  }
}

// Gets the camera transofmr as a pointer
static inline Transform* GetActiveTransformPtr(Registry* registry)
{
  DevDebug& devDebug = registry->GetEditorResource<DevDebug>();

  if (devDebug.activeCamera == CameraType::Scene)
  {
    return &(registry->GetComponent<Transform>(devDebug.sceneCameraId));
  }
  else if (devDebug.activeCamera == CameraType::Game)
  {
    return &(registry->GetComponent<Transform>(devDebug.gameCameraId));
  }
  else
  {
    std::cout << "Attempted to GetActiveTransformPtr a component that does not exist" << std::endl;
    exit(0);
  }
}

// Gets the camera transform as a reference
static inline Transform& GetActiveTransformRef(Registry* registry)
{
  DevDebug& devDebug = registry->GetEditorResource<DevDebug>();

  if (devDebug.activeCamera == CameraType::Scene)
  {
    return registry->GetComponent<Transform>(devDebug.sceneCameraId);
  }
  else if (devDebug.activeCamera == CameraType::Game)
  {
    return registry->GetComponent<Transform>(devDebug.gameCameraId);
  }
  else
  {
    std::cout << "Attempted to GetActiveTransformRef a component that does not exist" << std::endl;
    exit(0);
  }
}