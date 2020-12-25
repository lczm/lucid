#include "animationSystem.h"

AnimationSystem::AnimationSystem()
{
}

AnimationSystem::~AnimationSystem()
{
}

void AnimationSystem::Update(float dt, Registry* registry, Input* input)
{
  // TODO : This is assuming that every 'Model' is animated
  // There may be some models that are not animated.
  // A 'ModelAnimated' component should be created for those components
  // But for now this is enough for testing purposes.

  registry->GetComponentsIter<Model>()->Each([&](Model& model) {
    // For every mesh in every model, recalculate the animated bone transforms
    for (auto& mesh : model.GetMeshes())
    {
      // TODO : Set this apart
      // SetBoneTransform(dt, registry, mesh);
    }
  });
}

void AnimationSystem::SetBoneTransform(float dt, Registry* registry, Mesh& mesh)
{
  ShaderResource shaderResource = registry->GetComponent<ShaderResource>();

  std::vector<glm::mat4> transforms;
  BoneTransform(dt, mesh, transforms);

  // shaderResource.modelAnimatedShader.Bind();

  // shaderResource.modelAnimatedShader.SetUniformMatFloat4("gBones", transforms.size(),
  // transforms);
  // shaderResource.modelAnimatedShader.Unbind();
}

void AnimationSystem::BoneTransform(float dt, Mesh& mesh, std::vector<glm::mat4>& transforms)
{
  glm::mat4 identity = glm::mat4(1.0f);

  // TODO : currentAnimation
  const uint32_t currentAnimation = 0;
  uint32_t numPosKeys = mesh.scene->mAnimations[currentAnimation]->mChannels[0]->mNumPositionKeys;

  float animDuration = static_cast<float>(
      mesh.scene->mAnimations[currentAnimation]->mChannels[0]->mPositionKeys[numPosKeys - 1].mTime);

  float ticksPerSecond =
      static_cast<float>(mesh.scene->mAnimations[currentAnimation]->mTicksPerSecond != 0
                             ? mesh.scene->mAnimations[currentAnimation]->mTicksPerSecond
                             : 25.0f);

  // TODO : timeInSeconds
  const float timeInSeconds = 0.1f;
  float timeInTicks = timeInSeconds * ticksPerSecond;
  float animationTime = fmod(timeInTicks, animDuration);

  // TODO : ReadNodeHierarchy
  ReadNodeHierarchy(animationTime, mesh.scene, mesh.scene->mRootNode, identity);

  // TODO : mNumBones
  const int mNumBones = 4;
  transforms.resize(mNumBones);

  // TODO : Figure this out
  // for (size_t i = 0; i < mNumBones; i++)
  // {
  //   transforms[i] = m_BoneInfo[i].FinalTransformation;
  // }
}

void AnimationSystem::ReadNodeHierarchy(float animTime, const aiScene* scene, const aiNode* node,
                                        glm::mat4& parentTransform)
{
  // TODO : currentAnimation
  const uint32_t currentAnimation = 0;

  std::string nodeName(node->mName.data);
  const aiAnimation* pAnimation = scene->mAnimations[currentAnimation];

  glm::mat4 nodeTransformation = CastToGlmMat4(node->mTransformation);
  const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, nodeName);

  // If there is animation node found
  if (pNodeAnim)
  {
    // Interpolate scaling and generate scaling transformation matrix
    aiVector3D assimpScaling;
    // TODO : Linear Interpolation? (LERP)
    CalculateInterpolatedScaling(assimpScaling, animTime, pNodeAnim);

    glm::vec3 scale = glm::vec3(assimpScaling.x, assimpScaling.y, assimpScaling.z);
    glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), scale);

    // Interpolate rotation and generate rotation transformation matrix
    aiQuaternion assimpRotationQuat;
    CalculateInterpolatedRotation(assimpRotationQuat, animTime, pNodeAnim);
    glm::quat rotation = CastToGlmQuat(assimpRotationQuat);
    glm::mat4 rotationMatrix = glm::toMat4(rotation);

    // Interpolate translation and generate translation transformation matrix
    aiVector3D assimpTranslation;
    CalculateInterpolatedPosition(assimpTranslation, animTime, pNodeAnim);
    glm::vec3 translation =
        glm::vec3(assimpTranslation.x, assimpTranslation.y, assimpTranslation.z);
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);

    // Combine the transformations
    nodeTransformation = translationMatrix * rotationMatrix * scalingMatrix;
  }

  // Combine nodeTransformation with parentTransformation
  glm::mat4 globalTransformation = parentTransform * nodeTransformation;

  // TODO : m_BoneMapping?
  // TODO : mBoneInfo?
  // TODO : m_GlobalInverseTransform?
  // if (m_BoneMapping.find(nodeName) != m_BoneMapping.end())
  // {
  //   uint32_t boneIndex = m_BoneMapping[nodeName];
  //   m_BoneInfo[boneIndex].finalTransformation =
  //       m_GlobalInverseTransform * globalTransformation * m_BoneInfo[boneIndex].boneOffset;
  // }

  // Recursively apply bone transformation to the rest of the mesh children
  for (size_t i = 0; i < node->mNumChildren; i++)
  {
    ReadNodeHierarchy(animTime, scene, node->mChildren[i], globalTransformation);
  }
}

const aiNodeAnim* AnimationSystem::FindNodeAnim(const aiAnimation* pAnimation,
                                                const std::string nodeName)
{
  for (size_t i = 0; i < pAnimation->mNumChannels; i++)
  {
    const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];
    // If found by the nodeName given
    if (std::string(pNodeAnim->mNodeName.data) == nodeName)
    {
      return pNodeAnim;
    }
  }

  return nullptr;
}

uint32_t AnimationSystem::FindScaling(float animTime, const aiNodeAnim* pNodeAnim)
{
  assert(pNodeAnim->mNumScalingKeys > 0);

  for (size_t i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++)
  {
    if (animTime < static_cast<float>(pNodeAnim->mScalingKeys[i + 1].mTime))
    {
      return i;
    }
  }
  return 0;
}

uint32_t AnimationSystem::FindRotation(float animTime, const aiNodeAnim* pNodeAnim)
{
  assert(pNodeAnim->mNumRotationKeys > 0);

  for (size_t i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
  {
    if (animTime < static_cast<float>(pNodeAnim->mRotationKeys[i + 1].mTime))
    {
      return i;
    }
  }
  return 0;
}

uint32_t AnimationSystem::FindPosition(float animTime, const aiNodeAnim* pNodeAnim)
{
  for (size_t i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++)
  {
    if (animTime < static_cast<float>(pNodeAnim->mPositionKeys[i + 1].mTime))
    {
      return i;
    }
  }
  return 0;
}

void AnimationSystem::CalculateInterpolatedScaling(aiVector3D& vec, float animTime,
                                                   const aiNodeAnim* pNodeAnim)
{
  if (pNodeAnim->mNumScalingKeys == 1)
  {
    vec = pNodeAnim->mScalingKeys[0].mValue;
    return;
  }

  uint32_t scalingIndex = FindScaling(animTime, pNodeAnim);
  uint32_t nextScalingIndex = scalingIndex++;

  assert(nextScalingIndex < pNodeAnim->mNumScalingKeys);

  float dt = static_cast<float>(pNodeAnim->mScalingKeys[nextScalingIndex].mTime -
                                pNodeAnim->mScalingKeys[scalingIndex].mTime);
  float factor = (animTime - static_cast<float>(pNodeAnim->mScalingKeys[scalingIndex].mTime)) / dt;

  assert(factor >= 0.0f && factor <= 1.0f);

  const aiVector3D& start = pNodeAnim->mScalingKeys[scalingIndex].mValue;
  const aiVector3D& end = pNodeAnim->mScalingKeys[nextScalingIndex].mValue;
  aiVector3D delta = end - start;
  vec = start + factor * delta;
}
void AnimationSystem::CalculateInterpolatedRotation(aiQuaternion& quat, float animTime,
                                                    const aiNodeAnim* pNodeAnim)
{
  if (pNodeAnim->mNumPositionKeys == 1)
  {
    quat = pNodeAnim->mRotationKeys[0].mValue;
    return;
  }

  uint32_t rotationIndex = FindRotation(animTime, pNodeAnim);
  uint32_t nextRotationIndex = rotationIndex++;

  assert(nextRotationIndex < pNodeAnim->mNumRotationKeys);

  float dt = static_cast<float>(pNodeAnim->mRotationKeys[nextRotationIndex].mTime -
                                pNodeAnim->mRotationKeys[rotationIndex].mTime);
  float factor =
      (animTime - static_cast<float>(pNodeAnim->mRotationKeys[rotationIndex].mTime)) / dt;

  assert(factor >= 0.0f && factor <= 1.0f);

  const aiQuaternion& start = pNodeAnim->mRotationKeys[rotationIndex].mValue;
  const aiQuaternion& end = pNodeAnim->mRotationKeys[nextRotationIndex].mValue;

  // Interpolate between start and end given given
  aiQuaternion::Interpolate(quat, start, end, factor);

  // Normalize the quat
  quat = quat.Normalize();
}
void AnimationSystem::CalculateInterpolatedPosition(aiVector3D& vec, float animTime,
                                                    const aiNodeAnim* pNodeAnim)
{
  if (pNodeAnim->mNumPositionKeys == 1)
  {
    vec = pNodeAnim->mPositionKeys[0].mValue;
    return;
  }

  uint32_t positionIndex = FindPosition(animTime, pNodeAnim);
  uint32_t nextPositionIndex = positionIndex++;

  assert(nextPositionIndex < pNodeAnim->mNumPositionKeys);

  float dt = static_cast<float>(pNodeAnim->mPositionKeys[nextPositionIndex].mTime -
                                pNodeAnim->mPositionKeys[positionIndex].mTime);

  float factor =
      (animTime - static_cast<float>(pNodeAnim->mPositionKeys[positionIndex].mTime)) / dt;

  assert(factor >= 0.0f && factor <= 1.0f);

  const aiVector3D& start = pNodeAnim->mPositionKeys[positionIndex].mValue;
  const aiVector3D& end = pNodeAnim->mPositionKeys[nextPositionIndex].mValue;
  aiVector3D delta = end - start;
  vec = start + factor * delta;
}