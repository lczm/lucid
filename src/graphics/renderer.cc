#include "renderer.h"

Renderer::Renderer() = default;

Renderer::~Renderer() = default;

void Renderer::DrawMesh(Mesh& mesh, Shader& shader) {
  uint32_t diffuseNr = 1;
  uint32_t specularNr = 1;

  for (size_t i = 0; i < mesh.textures.size(); i++) {
    // Activate texture unit before binding
    glActiveTexture(GL_TEXTURE0 + i);

    // Retrieve texture number
    std::string number;
    std::string name = mesh.textures[i].type;

    if (name == "texture_diffuse") number = std::to_string(diffuseNr++);
    else if (name == "texture_specular") number = std::to_string(specularNr++);

    glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id);
    shader.SetUniformInt1(("material." + name + number).c_str(), i);
  }

  // Draw the mesh
  glBindVertexArray(mesh.VAO);
  glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  // Reset gl texture unit to 0
  glActiveTexture(GL_TEXTURE0);
}

void Renderer::DrawModel(Model& model, Shader& shader) {
  for (Mesh& mesh : model.GetMeshes()) {
    DrawMesh(mesh, shader);
  }
}