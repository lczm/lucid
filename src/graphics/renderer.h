#pragma once

#include <vector>

#include "gl.h"
#include "model.h"

class Renderer {
 private:
 public:
  Renderer();
  ~Renderer();

  // Public draw methods
  // The draw method for 3d models
  void DrawModel(Model& model);

  // Potential skeleton methods that needs to be implemented.
  void DrawQuad();
  void DrawTexturedQuad();

  /*
   * This is to start and end draw calls, this is mostly to help batch calls
   * together so that it is more performant.
   */
  void Start();
  void End();
};