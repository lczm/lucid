#pragma once

#include <limits>
#include <vector>
#include <string>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "gl.h"
#include "mesh.h"

struct BoundingBox {
  float minX = std::numeric_limits<float>::max();
  float maxX = std::numeric_limits<float>::min();

  float minY = std::numeric_limits<float>::max();
  float maxY = std::numeric_limits<float>::min();

  float minZ = std::numeric_limits<float>::max();
  float maxZ = std::numeric_limits<float>::min();
};

class Model {
 private:
  std::string tag;
  std::vector<MeshTexture> loadedTextures;
  std::vector<Mesh> meshes;
  std::string directory;

  BoundingBox boundingBox;

 public:
  Model();
  Model(std::string path);

  /*
   * TODO : Temporary or find a solution around this.
   * Needs a way to find a certain model in space through 'tags' mostly
   * debugging purposes.
   */
  void SetTag(std::string tag);
  std::string GetTag();

  std::vector<Mesh> GetMeshes();

  void SetBoundingBox(BoundingBox& boundingBox);
  BoundingBox GetBoundingBox();

  void LoadModel(std::string path);
  void ProcessNode(aiNode* node, const aiScene* scene);
  Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<MeshTexture> LoadMaterialTextures(aiMaterial* material, aiTextureType type,
                                                std::string typeName);

  uint32_t TextureFromFile(const char* path, const std::string& directory, bool gamma);
  void CalculateModelBoundingBox();
};