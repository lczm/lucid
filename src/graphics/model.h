#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <cereal/types/string.hpp>

#include "shader.h"
#include "mesh.h"
#include "boundingBox.h"
#include "renderUtils.h"

class Model
{
 public:
  Assimp::Importer* importer;
  const aiScene* scene;

  std::string path;
  std::string directory;

  std::vector<Mesh> meshes;
  std::vector<MeshTexture> loadedTextures;

  // Animation mapping
  std::map<std::pair<uint32_t, std::string>, uint32_t> animationMapping;
  std::unordered_map<std::string, uint32_t> animationIdMapping;

  // Bone name mappings
  std::unordered_map<std::string, uint32_t> boneMapping;

  std::vector<glm::mat4> boneMatrices;
  std::vector<glm::mat4> boneOffsets;

  // All the vertices of the model
  std::vector<glm::vec4> vertices;

  // Bounding box of the model, this is to help free up further computation
  BoundingBox boundingBox;

 public:  // State checks by the systems
  bool hasAnimations = false;
  bool toAnimate = false;

 public:
  Model();
  Model(std::string path);
  ~Model();

  void Load();

  std::vector<Mesh> GetMeshes();
  aiNode* GetRootNode();

  void ProcessNode(aiNode* node, const aiScene* scene);
  Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
  void UpdateBoneMatrices(float dt, uint32_t animationId, aiNode* node, glm::mat4 transform);

  std::vector<MeshTexture> LoadMaterialTextures(aiMaterial* material, aiTextureType type,
                                                std::string typeName);

  uint32_t TextureFromFile(const char* path, const std::string& directory, bool gamma);

  glm::mat4 InterpolateTranslationMatrix(float dt, aiVectorKey* keys, uint32_t n);
  glm::mat4 InterpolateRotationMatrix(float dt, aiQuatKey* keys, uint32_t n);
  glm::mat4 InterpolateScalingMatrix(float dt, aiVectorKey* keys, uint32_t n);

  template <class Archive>
  void serialize(Archive& archive)
  {
    archive(                   //
        CEREAL_NVP(path),      //
        CEREAL_NVP(toAnimate)  //
    );
  }
};
