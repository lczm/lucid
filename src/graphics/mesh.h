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

const uint32_t NUM_BONES_PER_VERTEX = 4;
struct VertexBoneData
{
  uint32_t ids[NUM_BONES_PER_VERTEX];
  float weights[NUM_BONES_PER_VERTEX];

  // TODO : Why this?
  VertexBoneData()
  {
    for (size_t i = 0; i < NUM_BONES_PER_VERTEX; i++)
    {
      ids[i] = 0;
      weights[i] = 0;
    }
  }
};

struct BoneInfo
{
  glm::mat4 finalTransformation = glm::mat4(0.0f);
  glm::mat4 boneOffset = glm::mat4(0.0f);
};

class Mesh
{
 public:
  uint32_t VAO, VBO, EBO, boneVBO;
  const aiScene* scene;
  std::vector<MeshVertex> vertices;
  std::vector<uint32_t> indices;
  std::vector<MeshTexture> textures;
  std::vector<VertexBoneData> bones;

  uint32_t numIndices = 0;
  uint32_t baseVertex = 0;
  uint32_t baseIndex = 0;

 public:
  Mesh(std::vector<MeshVertex> vertices, std::vector<uint32_t> indices,
       std::vector<MeshTexture> textures, std::vector<VertexBoneData> bones, const aiScene* scene,
       uint32_t meshNumIndices, uint32_t meshBaseVertex, uint32_t meshBaseIndex);

  void SetupMesh();
};