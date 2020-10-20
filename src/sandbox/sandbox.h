#pragma once

#include "gl.h"
#include "ecs.h"
#include "input.h"
#include "glm.hpp"
#include "gtx/string_cast.hpp"
#include "gtc/matrix_transform.hpp"

class Sandbox {
 private:
  Registry* registry;
  Input* input;
  GLFWwindow* window;

 public:
  Sandbox(Registry* registry, Input* input, GLFWwindow* window);
  ~Sandbox();

  void Update(double dt);
};