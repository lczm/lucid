#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <glad/gl.h>
#include <vector>

#include "stb_image.h"
#include "constants.h"
#include "glm.hpp"
#include "gtc/type_ptr.hpp"

typedef struct Vertex
{
  glm::vec2 position;
  glm::vec3 color;
  glm::vec2 texCoord;
} Vertex;

// Shader class
class Shader
{
 private:
  unsigned int id;

  std::string vertexSource;
  std::string fragmentSource;

  char* vSrc;
  char* fSrc;

  unsigned int vertexShader;
  unsigned int fragmentShader;

  void ReadFile(std::string& path, std::string& output);

 public:
  Shader();
  ~Shader();

  void Bind();
  void Unbind();
  void CreateShader(std::string vertexPath, std::string fragmentPath);
  unsigned int GetId()
  {
    return id;
  };

 public:  // Uniform methods // TODO : Cache/Memoize uniform names
  void SetUniformInt1(const std::string& name, int v);
  void SetUniformFloat1(const std::string& name, float v);

  void SetUniformInt2(const std::string& name, int v1, int v2);
  void SetUniformFloat2(const std::string& name, float v1, float v2);

  void SetUniformInt3(const std::string& name, int v1, int v2, int v3);
  void SetUniformFloat3(const std::string& name, float v1, float v2, float v3);

  void SetUniformInt4(const std::string& name, int v1, int v2, int v3, int v4);
  void SetUniformFloat4(const std::string& name, float v1, float v2, float v3, float v4);

  void SetUniformVecFloat3(const std::string& name, glm::vec3 v3);
  void SetUniformMatFloat4(const std::string& name, glm::mat4 mat);
  void SetUniformMatFloat4(const std::string& name, uint32_t count, std::vector<glm::mat4> mats);
};

struct ShaderResource
{
  Shader modelShader;
  Shader modelAnimatedShader;

  // Generic primitive shader
  Shader primitiveShader;

  Shader lineShader;
  Shader cubeShader;
  Shader sphereShader;
  Shader fontShader;
};