#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "component.h"

/*
 * TODO : Update this to the quaternion based representation
 */

enum class DrawType
{
  Line,    //
  Sphere,  //
  Cube,    //
  Model,   //
};

static glm::mat4 GetModelMatrix(Transform transform)
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