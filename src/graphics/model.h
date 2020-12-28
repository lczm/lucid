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

// TODO : Figure this out as well
class Namer
{
 private:
  std::map<std::string, uint32_t> map;
  uint32_t total;

 public:
  Namer();
  ~Namer();

  uint32_t Name(const std::string& name);
  uint32_t Total() const;
  std::map<std::string, uint32_t>& Map();
  void Clear();
};

class Model
{
 public:
  Assimp::Importer* importer;
  const aiScene* scene;

  std::vector<glm::vec4> vertices;
  std::vector<MeshTexture> loadedTextures;
  std::vector<Mesh> meshes;
  std::string directory;

  // Bone processing maps
  // Maps a bone name to the its
  std::unordered_map<std::string, uint32_t> boneMapping;

  // Animation mapping
  std::map<std::pair<uint32_t, std::string>, uint32_t> animationMapping;

  // Bone name mappings
  Namer boneNamer;

  std::vector<glm::mat4> boneMatrices;
  std::vector<glm::mat4> boneOffsets;

 public:
  Model();
  Model(std::string path);
  ~Model();

  std::vector<Mesh> GetMeshes();

  void ProcessNode(aiNode* node, const aiScene* scene);
  Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
  void UpdateBoneMatrices(float dt, uint32_t animationId, aiNode* node, glm::mat4 transform);

  std::vector<MeshTexture> LoadMaterialTextures(aiMaterial* material, aiTextureType type,
                                                std::string typeName);

  uint32_t TextureFromFile(const char* path, const std::string& directory, bool gamma);

  glm::mat4 InterpolateTranslationMatrix(float dt, aiVectorKey* keys, uint32_t n);
  glm::mat4 InterpolateRotationMatrix(float dt, aiQuatKey* keys, uint32_t n);
  glm::mat4 InterpolateScalingMatrix(float dt, aiVectorKey* keys, uint32_t n);
};
