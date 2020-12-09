#pragma once

#include <math.h>
#include <vector>
#include <chrono>

#include "gl.h"
#include "model.h"
#include "cube.h"
#include "line.h"
#include "sphere.h"
#include "boundingBox.h"
#include "gtx/string_cast.hpp"

class Renderer {
 private:
  // uint32_t batchIndexCount = 0;
  // std::vector<glm::mat4> modelMatrices;

 public:
  Renderer();
  ~Renderer();

  // Public draw methods
  // The draw method for 3d models
  void DrawMesh(Mesh& mesh, Shader& shader);
  void DrawModel(Model& model, Shader& shader);

  void DrawCube(Cube& cube, Shader& shader);
  void DrawSphere(Sphere& sphere, Shader& shader);

  void DrawTexturedCube(Cube& cube, Shader& shader);
  void DrawTexturedSphere(Sphere& sphere, Shader& shader);

  void DrawLine(Line& line, Shader& shader);
  void DrawLine(glm::vec3 origin, glm::vec3 end, glm::vec3 color);

  // Potential skeleton methods that needs to be implemented.
  void DrawQuad();
  void DrawTexturedQuad();

  void DrawBoundingBox(Model& model, Shader& shader);
  void DrawBoundingBox(Sphere& sphere, Shader& shader);
  void DrawBoundingBox(ColliderCube colliderCube, Shader& shader);

  void CalculateModelBoundingBox(Model& model);

  /*
   * This is to start and end draw calls, this is mostly to help batch calls
   * together so that it is more performant.
   */
  void Start();
  void Flush();
};