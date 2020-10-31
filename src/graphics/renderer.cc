#include "renderer.h"

Renderer::Renderer() {
  glGenVertexArrays(1, &bbVAO);
  glBindVertexArray(bbVAO);

  glGenBuffers(1, &bbVBO);
  glBindBuffer(GL_ARRAY_BUFFER, bbVBO);

  bbShader.CreateShader(TRIANGLE_VERTEX_SHADER, TRIANGLE_FRAGMENT_SHADER);
};

Renderer::~Renderer() = default;

void Renderer::DrawMesh(Mesh& mesh, Shader& shader) {
  shader.Bind();
  uint32_t diffuseNr = 1;
  uint32_t specularNr = 1;

  for (size_t i = 0; i < mesh.textures.size(); i++) {
    // Activate texture unit before binding
    glActiveTexture(GL_TEXTURE0 + i);

    // Retrieve texture number
    std::string number;
    std::string name = mesh.textures[i].type;

    if (name == "texture_diffuse")
      number = std::to_string(diffuseNr++);
    else if (name == "texture_specular")
      number = std::to_string(specularNr++);

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

  BoundingBox boundingBox = CalculateModelBoundingBox(model);
  DrawBoundingBox(boundingBox);
  return;
}

void Renderer::DrawBoundingBox(BoundingBox& boundingBox) {
  float vertices[] = {
      // x , y, z , r, g, b
      boundingBox.minX, boundingBox.minY, boundingBox.minZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.maxX, boundingBox.minY, boundingBox.minZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.maxX, boundingBox.maxY, boundingBox.minZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.maxX, boundingBox.maxY, boundingBox.minZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.minX, boundingBox.maxY, boundingBox.minZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.minX, boundingBox.minY, boundingBox.minZ, 1.0f, 1.0f, 1.0f,  //

      boundingBox.minX, boundingBox.minY, boundingBox.maxZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.maxX, boundingBox.minY, boundingBox.maxZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.maxX, boundingBox.maxY, boundingBox.maxZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.maxX, boundingBox.maxY, boundingBox.maxZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.minX, boundingBox.maxY, boundingBox.maxZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.minX, boundingBox.minY, boundingBox.maxZ, 1.0f, 1.0f, 1.0f,  //

      boundingBox.minX, boundingBox.maxY, boundingBox.maxZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.minX, boundingBox.maxY, boundingBox.minZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.minX, boundingBox.minY, boundingBox.minZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.minX, boundingBox.minY, boundingBox.minZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.minX, boundingBox.minY, boundingBox.maxZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.minX, boundingBox.maxY, boundingBox.maxZ, 1.0f, 1.0f, 1.0f,  //

      boundingBox.maxX, boundingBox.maxY, boundingBox.maxZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.maxX, boundingBox.maxY, boundingBox.minZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.maxX, boundingBox.minY, boundingBox.minZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.maxX, boundingBox.minY, boundingBox.minZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.maxX, boundingBox.minY, boundingBox.maxZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.maxX, boundingBox.maxY, boundingBox.maxZ, 1.0f, 1.0f, 1.0f,  //

      boundingBox.minX, boundingBox.minY, boundingBox.minZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.maxX, boundingBox.minY, boundingBox.minZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.maxX, boundingBox.minY, boundingBox.maxZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.maxX, boundingBox.minY, boundingBox.maxZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.minX, boundingBox.minY, boundingBox.maxZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.minX, boundingBox.minY, boundingBox.minZ, 1.0f, 1.0f, 1.0f,  //

      boundingBox.minX, boundingBox.maxY, boundingBox.minZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.maxX, boundingBox.maxY, boundingBox.minZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.maxX, boundingBox.maxY, boundingBox.maxZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.maxX, boundingBox.maxY, boundingBox.maxZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.minX, boundingBox.maxY, boundingBox.maxZ, 1.0f, 1.0f, 1.0f,  //
      boundingBox.minX, boundingBox.maxY, boundingBox.minZ, 1.0f, 1.0f, 1.0f,  //
  };

  uint32_t indices[] = {
      0, 1, 3,  // first triangle
      1, 2, 3   // second triangle
  };

  bbShader.Bind();
  glBindVertexArray(bbVAO);
  glBindBuffer(GL_ARRAY_BUFFER, bbVBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glLineWidth(1.0f);
  glDrawArrays(GL_LINE_LOOP, 0, 24);
  bbShader.Unbind();
}

BoundingBox Renderer::CalculateModelBoundingBox(Model& model) {
  std::vector<Mesh> meshes = model.GetMeshes();

  BoundingBox boundingBox;

  for (size_t i = 0; i < meshes.size(); i++) {
    std::vector<MeshVertex>& vertices = meshes[i].vertices;
    for (size_t j = 0; j < vertices.size(); j++) {
      boundingBox.minX = glm::min(boundingBox.minX, vertices[j].position.x);
      boundingBox.maxX = glm::max(boundingBox.maxX, vertices[j].position.x);

      boundingBox.minY = glm::min(boundingBox.minY, vertices[j].position.y);
      boundingBox.maxY = glm::max(boundingBox.maxY, vertices[j].position.y);

      boundingBox.minZ = glm::min(boundingBox.minZ, vertices[j].position.z);
      boundingBox.maxZ = glm::max(boundingBox.maxZ, vertices[j].position.z);
    }
  }

  return boundingBox;
}
