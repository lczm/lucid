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

  DrawBoundingBox(model.GetBoundingBox());
}

void Renderer::DrawCube(Cube& cube, Shader& shader) {
  glBindVertexArray(cube.VAO);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube.EBO);
  glDrawElements(GL_TRIANGLES, cube_indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Renderer::DrawSphere(Sphere& sphere, Shader& shader) {
  glBindVertexArray(sphere.VAO);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere.EBO);
  glDrawElements(GL_TRIANGLES, sphere.indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Renderer::DrawLine(Line& line, Shader& shader) {
  glBindVertexArray(line.VAO);
  glDrawArrays(GL_LINES, 0, 2);
  glBindVertexArray(0);
}

void Renderer::DrawBoundingBox(BoundingBox& boundingBox) {
  // TODO : This should be using index buffers...
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

  // bbShader.Bind();
  glBindVertexArray(bbVAO);
  glBindBuffer(GL_ARRAY_BUFFER, bbVBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glLineWidth(1.0f);
  glDrawArrays(GL_LINE_LOOP, 0, 24);
  // bbShader.Unbind();
}
