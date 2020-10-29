#pragma once

#include <math.h>
#include <vector>
#include <limits>

#include "gl.h"
#include "model.h"
#include "gtx/string_cast.hpp"

struct BoundingBox {
  float minX = std::numeric_limits<float>::max();
  float maxX = std::numeric_limits<float>::min();

  float minY = std::numeric_limits<float>::max();
  float maxY = std::numeric_limits<float>::min();

  float minZ = std::numeric_limits<float>::max();
  float maxZ = std::numeric_limits<float>::min();
};

class Renderer {
 private:
  uint32_t VAO;
  uint32_t VBO;
  Shader shader;

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
  BoundingBox CalculateModelBoundingBox(Model& model);

  /*
   * This is to start and end draw calls, this is mostly to help batch calls
   * together so that it is more performant.
   */
  void Start();
  void End();
};