#pragma once

#include <math.h>
#include <vector>
#include <chrono>

#include "gl.h"
#include "model.h"
#include "gtx/string_cast.hpp"

class Renderer {
 private:
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