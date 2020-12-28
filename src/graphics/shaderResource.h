#pragma once

#include "shader.h"

// Note : this is just a temporary work around, there should be a better way to manage shaders
struct ShaderResource
{
  Shader modelShader;
  Shader modelAnimatedShader;

  // Generic primitive shader
  Shader primitiveShader;

  Shader lineShader;
  Shader cubeShader;
  Shader sphereShader;
};