#pragma once

#include <math.h>
#include <vector>
#include <chrono>

#include "gl.h"
#include "ecs.h"
#include "model.h"
#include "cube.h"
#include "line.h"
#include "sphere.h"
#include "boundingBox.h"
#include "primitiveVertex.h"
#include "renderUtils.h"
#include "engineComponents.h"
#include "dbg.h"

#include "gtx/string_cast.hpp"

const uint32_t MAX_BUFFER = 1000;

class Renderer
{
 private:
  Registry* registry;

  uint32_t batchIndexCount = 0;
  std::vector<glm::mat4> modelMatrices;
  std::vector<LineVertex> linePrimitiveBuffer;
  std::vector<CubeVertex> cubePrimititiveBuffer;
  std::vector<SphereVertex> spherePrimitiveBuffer;

 public:
  Renderer(Registry* registry);
  ~Renderer();

  // Public draw methods
  // The draw method for 3d models
  void DrawMesh(Mesh& mesh, Shader& shader);
  void DrawModel(Model& model, Shader& shader);

  void DrawCube(Cube& cube, Shader& shader);
  void DrawCubeIndexed(const PrimitiveBatchIds primitiveBatchIds);

  void DrawSphere(Sphere& sphere, Shader& shader);
  void DrawSphereIndexed(const PrimitiveBatchIds);

  // void DrawLine(Line& line, Shader& shader);
  void DrawLineIndexed(const PrimitiveBatchIds primitiveBatchIds);

  void DrawBoundingBox(Model& model, Shader& shader);
  void DrawBoundingBox(Sphere& sphere, Shader& shader);
  void DrawBoundingBox(ColliderCube colliderCube, Shader& shader);

  void CalculateModelBoundingBox(Model& model);

  /*
   * This is to start and end draw calls, this is mostly to help batch calls
   * together so that it is more performant.
   */
  void StartBatch();
  void FlushBatch(PrimitiveBatchIds primitiveBatchIds, DrawType drawType);

 public:
  void PushLineBuffer(const glm::mat4 modelMatrix, const Line line);
  void PushSphereBuffer(const glm::mat4 modelMatrix, const Sphere sphere);
  void PushCubeBuffer(const glm::mat4 modelMatrix, const Cube cube);
  void PushModelBuffer(const glm::mat4 modelMatrix, const Model model);
};