#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "shader.h"
#include "mesh.h"
#include "boundingBox.h"
#include "renderUtils.h"

const uint32_t NUM_BONES_PER_VERTEX = 4;
struct VertexBone
{
  uint32_t ids[NUM_BONES_PER_VERTEX];
  float weights[NUM_BONES_PER_VERTEX];
};

struct BoneInfo
{
  glm::mat4 finalTransformation = glm::mat4(1.0f);
  glm::mat4 boneOffset = glm::mat4(1.0f);
};

class Model
{
 public:
  const aiScene* scene;
  std::vector<MeshTexture> loadedTextures;
  std::vector<Mesh> meshes;
  std::string directory;

  uint32_t boneCount = 0;
  std::vector<VertexBone> bones;

 public:
  Model();
  Model(std::string path);

  std::vector<Mesh> GetMeshes();

  void ProcessNode(aiNode* node, const aiScene* scene);
  Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
  void ProcessBones(aiMesh* mesh, const aiScene* scene, uint32_t meshIndex);
  std::vector<MeshTexture> LoadMaterialTextures(aiMaterial* material, aiTextureType type,
                                                std::string typeName);

  uint32_t TextureFromFile(const char* path, const std::string& directory, bool gamma);
};