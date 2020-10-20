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

typedef struct Vertex {
  glm::vec2 position;
  glm::vec3 color;
  glm::vec2 texCoord;
} Vertex;

// Globally used double triangle to make a quad indices
const unsigned int indices[] = {0, 1, 2, 2, 3, 0};

// Shader class
class Shader {
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
  unsigned int GetId() {
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
  void SetUniformFloat4(const std::string& name, float v1, float v2, float v3,
                        float v4);

  void SetUniformMatFloat4(const std::string& name, glm::mat4 mat);
};

// VertexBufferObjects (VBOs)
class VertexBuffer {
 private:
  unsigned int id;

 public:
  VertexBuffer();
  ~VertexBuffer();

  void Bind();
  void Unbind();
  void BindData(const void* data, unsigned int size);

  unsigned int GetId() {
    return id;
  };
  void Generate() {
    glGenBuffers(1, &id);
  };
};

// IndexBuffers
class IndexBuffer {
 private:
  unsigned int id;
  unsigned int count;

 public:
  IndexBuffer();
  ~IndexBuffer();

  void Bind();
  void Unbind();
  void BindData(const unsigned int* data, unsigned int count);
  unsigned int GetCount();
  unsigned int GetId();
  void Generate() {
    glGenBuffers(1, &id);
  };
};

// VertexBufferLayout
struct VertexBufferElement {
  unsigned int type;
  unsigned int count;
  unsigned char normalized;

  static unsigned int GetSizeOfType(unsigned int type) {
    switch (type) {
      case GL_FLOAT:
        return 4;
      case GL_UNSIGNED_INT:
        return 4;
      case GL_UNSIGNED_BYTE:
        return 1;
    }
    return 0;
  }
};

class VertexBufferLayout {
 private:
  unsigned int stride;
  std::vector<VertexBufferElement> elements;

 public:
  VertexBufferLayout();
  ~VertexBufferLayout();

  // TODO : Support other types other than floats
  // Note : This push function assumes all incoming is float
  void Push(unsigned int count);

  std::vector<VertexBufferElement>& GetElements() {
    return elements;
  }
  unsigned int GetStride() {
    return stride;
  }
};

class VertexArray {
 private:
  unsigned int id;

 public:
  VertexArray();
  ~VertexArray();

  void Bind();
  void Unbind();
  void AddBuffer(VertexBuffer& vb, VertexBufferLayout& layout);

  unsigned int GetId() {
    return id;
  };
  void Generate() {
    glGenVertexArrays(1, &id);
  }
};

// Texture
class Texture {
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

  int GetWidth() {
    return width;
  };
  int GetHeight() {
    return height;
  };

  int GetSsTotal() {
    return ssTotal;
  };
  int GetSsRow() {
    return ssRow;
  };
  int GetSsCol() {
    return ssCol;
  };
  int GetSsIter() {
    return ssIter;
  };
  int GetSsWidth() {
    return ssWidth;
  };
  int GetSsHeight() {
    return ssHeight;
  };

  unsigned int GetId() {
    return id;
  };
  void Generate() {
    glGenTextures(1, &id);
  };
};
