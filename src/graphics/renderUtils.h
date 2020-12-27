#pragma once

#include "ecs.h"
#include "input.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtx/quaternion.hpp"
#include "gtc/type_ptr.hpp"
#include "component.h"
#include "engineComponents.h"
#include "quatCamera.h"

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
static glm::mat4 GetModelMatrix(const Transform transform)
{
  glm::mat4 matrixModel = glm::mat4(1.0f);
  glm::mat4 rotationMatrix = glm::mat4(1.0f);

  matrixModel = glm::translate(matrixModel, transform.position);
  matrixModel = glm::scale(matrixModel, transform.scale);

  // Rotation matrix
  rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[0], glm::vec3(1.0, 0.0, 0.0));
  rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[1], glm::vec3(0.0, 1.0, 0.0));
  rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[2], glm::vec3(0.0, 0.0, 1.0));

  matrixModel *= rotationMatrix;

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

static glm::vec3 GetRayDirection(Registry* registry, Input* input)
{
#if DEBUG
  WidgetLayout& widgetLayout = registry->GetComponent<WidgetLayout>();
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

  // lucid::Log(x, " ", y, " ", z);

  // normalized device coordinates
  glm::vec3 rayNds = glm::vec3(x, y, z);

  // homogeneous clip coordinates
  glm::vec4 rayClip = glm::vec4(rayNds.x, rayNds.y, -1.0f, 1.0f);

  QuatCamera& quatCamera = registry->GetComponent<QuatCamera>();

  // convert to eye/camera coordinates
  glm::vec4 rayEye = glm::inverse(quatCamera.GetProjection()) * rayClip;

  // unproject the x, z part
  rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

  // 4d world coordinates
  // normalize the vector as well
  glm::vec3 rayWorld = glm::vec3(glm::normalize(glm::inverse(quatCamera.GetView()) * rayEye));
  // lucid::Log(glm::to_string(rayWorld));

  // Scale this by a fairly huge amount
  rayWorld *= 1000.0f;

  // Re-inverse the y-values since input->GetMouseY() {abs(SCREEN_HEIGHT - y)}
  // TODO : Find out why i have to invert this for whatever reason...

  // rayWorld.x = -rayWorld.x;
  // rayWorld.y = rayWorld.y;
  // rayWorld.z = -15.0f;

  return rayWorld;
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

// TODO : Check if this is correct
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