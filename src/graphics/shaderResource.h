#pragma once

#include "gl.h"

// Note : this is just a temporary work around, there should be a better way to manage shaders
struct ShaderResource
{
  Shader modelShader;
  Shader triangleShader;
  Shader primitiveShader;
  Shader primitiveShaderBatch;
  Shader cubeShaderBatch;
  Shader boundingBoxShader;
};