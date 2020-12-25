#pragma once

#include <string>
#include <iostream>

#include "glm.hpp"
#include "gtc/type_ptr.hpp"

#include "ecs.h"
#include "component.h"
#include "engineComponents.h"
#include "mesh.h"
#include "model.h"
#include "shader.h"
#include "shaderResource.h"

class AnimationSystem : public System
{
 public:
  AnimationSystem();
  ~AnimationSystem();

  void Update(float dt, Registry* registry, Input* input);

  void SetBoneTransform(float dt, Registry* registry, Mesh& mesh);
  void BoneTransform(float dt, Mesh& mesh, std::vector<glm::mat4>& transforms);
  void ReadNodeHierarchy(float animTime, const aiScene* scene, const aiNode* node,
                         glm::mat4& parentTransform);
  const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string nodeName);

  uint32_t FindScaling(float animTime, const aiNodeAnim* pNodeAnim);
  uint32_t FindRotation(float animTime, const aiNodeAnim* pNodeAnim);
  uint32_t FindPosition(float animTime, const aiNodeAnim* pNodeAnim);

  void CalculateInterpolatedScaling(aiVector3D& vec, float animTime, const aiNodeAnim* pNodeAnim);
  void CalculateInterpolatedRotation(aiQuaternion& quat, float animTime,
                                     const aiNodeAnim* pNodeAnim);
  void CalculateInterpolatedPosition(aiVector3D& vec, float animTime, const aiNodeAnim* pNodeAnim);
};