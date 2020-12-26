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

class Model
{
 public:
  Assimp::Importer* importer;
  const aiScene* scene;

  std::vector<MeshTexture> loadedTextures;
  std::vector<Mesh> meshes;
  std::string directory;

  // TODO : Check if this is actually needed?
  uint32_t boneCount = 0;
  std::vector<BoneInfo> boneInfo;

  // TODO : Check this out as well?
  uint32_t numVertices = 0;
  uint32_t numIndices = 0;

 public:
  Model();
  Model(std::string path);
  ~Model();

  std::vector<Mesh> GetMeshes();

  void ProcessNode(aiNode* node, const aiScene* scene);
  Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<MeshTexture> LoadMaterialTextures(aiMaterial* material, aiTextureType type,
                                                std::string typeName);

  uint32_t TextureFromFile(const char* path, const std::string& directory, bool gamma);
};