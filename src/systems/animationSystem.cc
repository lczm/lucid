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

  uint32_t modelCount = 0;
  registry->GetComponentsIter<Model>()->Each([&](Model& model) {
    modelCount++;  //
  });
  // std::cout << "Models : " << modelCount << std::endl;
}

void AnimationSystem::SetBoneTransform(float dt, Registry* registry, Model& model)
{
  ShaderResource shaderResource = registry->GetComponent<ShaderResource>();

  std::vector<glm::mat4> transforms;

  shaderResource.modelAnimatedShader.Bind();

  shaderResource.modelAnimatedShader.SetUniformMatFloat4("gBones", transforms.size(), transforms);
  shaderResource.modelAnimatedShader.Unbind();
}

void AnimationSystem::BoneTransform(float dt, std::vector<glm::mat4> transforms)
{
}