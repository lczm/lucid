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

  void readFile(std::string& path, std::string& output);

 public:
  Shader();
  ~Shader();

  void bind();
  void unbind();
  void createShader(std::string vertexPath, std::string fragmentPath);
  unsigned int getId() {
    return id;
  };

 public:  // Uniform methods // TODO : Cache/Memoize uniform names
  void setUniformInt1(const std::string& name, int v);
  void setUniformFloat1(const std::string& name, float v);

  void setUniformInt2(const std::string& name, int v1, int v2);
  void setUniformFloat2(const std::string& name, float v1, float v2);

  void setUniformInt3(const std::string& name, int v1, int v2, int v3);
  void setUniformFloat3(const std::string& name, float v1, float v2, float v3);

  void setUniformInt4(const std::string& name, int v1, int v2, int v3, int v4);
  void setUniformFloat4(const std::string& name, float v1, float v2, float v3,
                        float v4);

  void setUniformMatFloat4(const std::string& name, glm::mat4 mat);
};

// VertexBufferObjects (VBOs)
class VertexBuffer {
 private:
  unsigned int id;

 public:
  VertexBuffer();
  ~VertexBuffer();

  void bind();
  void unbind();
  void bindData(const void* data, unsigned int size);

  unsigned int getId() {
    return id;
  };
  void generate() {
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

  void bind();
  void unbind();
  void bindData(const unsigned int* data, unsigned int count);
  unsigned int getCount();
  unsigned int getId();
  void generate() {
    glGenBuffers(1, &id);
  };
};

// VertexBufferLayout
struct VertexBufferElement {
  unsigned int type;
  unsigned int count;
  unsigned char normalized;

  static unsigned int getSizeOfType(unsigned int type) {
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
  void push(unsigned int count);

  std::vector<VertexBufferElement>& getElements() {
    return elements;
  }
  unsigned int getStride() {
    return stride;
  }
};

class VertexArray {
 private:
  unsigned int id;

 public:
  VertexArray();
  ~VertexArray();

  void bind();
  void unbind();
  void addBuffer(VertexBuffer& vb, VertexBufferLayout& layout);

  unsigned int getId() {
    return id;
  };
  void generate() {
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

  void initialize(const char imagePath[], int slot = 0);

  void bind();
  void unbind();

  int getWidth() {
    return width;
  };
  int getHeight() {
    return height;
  };

  int getSsTotal() {
    return ssTotal;
  };
  int getSsRow() {
    return ssRow;
  };
  int getSsCol() {
    return ssCol;
  };
  int getSsIter() {
    return ssIter;
  };
  int getSsWidth() {
    return ssWidth;
  };
  int getSsHeight() {
    return ssHeight;
  };

  unsigned int getId() {
    return id;
  };
  void generate() {
    glGenTextures(1, &id);
  };
};
