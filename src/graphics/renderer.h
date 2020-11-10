#pragma once

#include <math.h>
#include <vector>
#include <chrono>

#include "gl.h"
#include "model.h"
#include "cube.h"
#include "sphere.h"
#include "boundingBox.h"
#include "gtx/string_cast.hpp"

class Renderer {
 private:
  // Note : Remove this down the line
  uint32_t bbVAO;
  uint32_t bbVBO;
  Shader bbShader;

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

  // Draw lines might be abit difficult to do...
  void DrawLine();
  void DrawLine(glm::vec3 vector, Shader& shader);

  // Potential skeleton methods that needs to be implemented.
  void DrawQuad();
  void DrawTexturedQuad();

  void DrawBoundingBox(BoundingBox& boundingBox);
  void CalculateModelBoundingBox(Model& model);

  /*
   * This is to start and end draw calls, this is mostly to help batch calls
   * together so that it is more performant.
   */
  void Start();
  void End();
};