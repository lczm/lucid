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
};

// Texture
class Texture
{
 private:
  unsigned int id = 8;

  // Variables that will be used for image loading
  int width;
  int height;
  int nrChannels;
  unsigned char* data;

  // Sprite sheet variables
  int ssTotal;   // Number of images
  int ssRow;     // No of rows
  int ssCol;     // No of columns per row
  int ssIter;    // Iterator to go through sheet
  int ssWidth;   // Width per image on sheet
  int ssHeight;  // Height per image on sheet
 public:
  Texture();
  Texture(const char imagePath[], int slot = 0);
  ~Texture();

  void Initialize(const char imagePath[], int slot = 0);

  void Bind();
  void Unbind();

  int GetWidth()
  {
    return width;
  };
  int GetHeight()
  {
    return height;
  };

  int GetSsTotal()
  {
    return ssTotal;
  };
  int GetSsRow()
  {
    return ssRow;
  };
  int GetSsCol()
  {
    return ssCol;
  };
  int GetSsIter()
  {
    return ssIter;
  };
  int GetSsWidth()
  {
    return ssWidth;
  };
  int GetSsHeight()
  {
    return ssHeight;
  };

  unsigned int GetId()
  {
    return id;
  };
  void Generate()
  {
    glGenTextures(1, &id);
  };
};
