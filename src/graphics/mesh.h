#pragma once

#include <string>
#include <vector>

#include "gl.h"
#include "glm.hpp"

struct MeshVertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
};

struct MeshTexture {
  uint32_t id;
  std::string type;
  std::string path;
};

class Mesh {
 private:
  uint32_t VAO, VBO, EBO;

 public:
  std::vector<MeshVertex> vertices;
  std::vector<uint32_t> indices;
  std::vector<MeshTexture> textures;

  Mesh(std::vector<MeshVertex> vertices,
       std::vector<uint32_t> indices,
       std::vector<MeshTexture> textures);

  void SetupMesh();
  void Draw(Shader &shader);
};