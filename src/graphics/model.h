#pragma once

#include <vector>
#include <string>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "gl.h"
#include "mesh.h"

class Model {
 private:
  std::vector<MeshTexture> loadedTextures;
  std::vector<Mesh> meshes;
  std::string directory;

 public:
  Model(std::string path);

  void LoadModel(std::string path);
  void ProcessNode(aiNode* node, const aiScene* scene);
  Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<MeshTexture> LoadMaterialTextures(aiMaterial* material,
                                                aiTextureType type,
                                                std::string typeName);

  void Draw(Shader& shader);

  uint32_t TextureFromFile(const char* path, const std::string& directory,
                           bool gamma);
};