#pragma once

#include <string>
#include <vector>

#include "shader.h"
#include "glm.hpp"
#include <assimp/scene.h>

struct MeshVertex
{
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
};

struct MeshTexture
{
  uint32_t id;
  std::string type;
  std::string path;
};

class Mesh
{
 public:
  uint32_t VAO, VBO, EBO;
  const aiScene* scene;
  std::vector<MeshVertex> vertices;
  std::vector<uint32_t> indices;
  std::vector<MeshTexture> textures;

 public:
  Mesh(std::vector<MeshVertex> vertices, std::vector<uint32_t> indices,
       std::vector<MeshTexture> textures, const aiScene* scene);

  void SetupMesh();
};