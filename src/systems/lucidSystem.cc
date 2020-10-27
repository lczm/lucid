#include "lucidSystem.h"

LucidSystem::LucidSystem() {
  modelShader.CreateShader(MODEL_VERTEX_SHADER, MODEL_FRAGMENT_SHADER);
}

LucidSystem::~LucidSystem() {
}

void LucidSystem::Update(double dt, Registry* registry, Input* input) {
  std::vector<void*> modelComponents = registry->GetComponents<Model>();
  std::vector<void*> shaderComponents = registry->GetComponents<Shader>();

  auto* models = static_cast<ComponentVector<Model>*>(modelComponents[0]);
  auto* shaders = static_cast<ComponentVector<Shader>*>(shaderComponents[0]);

  Shader* shader = shaders->At(0);
  shader->Bind();

  for (size_t i = 0; i < models->Size(); i++) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0f, 1.0f, 1.0f * (i * 5)));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    shader->SetUniformMatFloat4("model", model);

    models->At(i)->Draw(*shader);
  }
}