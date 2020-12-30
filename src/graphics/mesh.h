#pragma once

#include <string>
#include <vector>

#include "shader.h"
#include "glm.hpp"
#include <assimp/scene.h>
#include <algorithm>

const uint32_t NUM_MAX_BONES_PER_VERTEX = 4;
struct MeshVertex
{
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;

  // int boneIds[NUM_MAX_BONES_PER_VERTEX] = {0, 0, 0, 0};
  // float boneWeights[NUM_MAX_BONES_PER_VERTEX] = {0.0f, 0.0f, 0.0f, 0.0f};

  int boneIds[NUM_MAX_BONES_PER_VERTEX];
  float boneWeights[NUM_MAX_BONES_PER_VERTEX];

  MeshVertex()
  {
    for (size_t i = 0; i < NUM_MAX_BONES_PER_VERTEX; i++)
    {
      boneIds[i] = 0;
      boneWeights[i] = 0;
    }
  }

  void AddBone(int id, float weight)
  {
    for (size_t i = 0; i < NUM_MAX_BONES_PER_VERTEX; i++)
    {
      if (boneWeights[i] == 0)
      {
        boneIds[i] = id;
        boneWeights[i] = weight;
        return;
      }
    }
  }
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