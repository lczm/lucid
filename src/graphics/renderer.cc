#include "renderer.h"

Renderer::Renderer(){
    // glGenVertexArrays(1, &bbVAO);
    // glBindVertexArray(bbVAO);

    // glGenBuffers(1, &bbVBO);
    // glBindBuffer(GL_ARRAY_BUFFER, bbVBO);

    // bbShader.CreateShader(TRIANGLE_VERTEX_SHADER, TRIANGLE_FRAGMENT_SHADER);
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
}

void Renderer::DrawCube(Cube& cube, Shader& shader) {
  glBindVertexArray(cube.VAO);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube.EBO);
  glDrawElements(GL_TRIANGLES, cubeIndices.size(), GL_UNSIGNED_INT, 0);
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

void Renderer::DrawLine(glm::vec3 origin, glm::vec3 end, glm::vec3 color) {
}

void Renderer::DrawBoundingBox(Model& model, Shader& shader) {
}

void Renderer::DrawBoundingBox(Sphere& sphere, Shader& shader) {
  // glGenVertexArrays(1, &VAO);
  // glGenBuffers(1, &VBO);
  // glGenBuffers(1, &EBO);

  // // Bind VAO & VBO
  // glBindVertexArray(VAO);
  // glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // // Set VBO data
  // glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

  // // Bind EBO
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  // // Set EBO data
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0],
  //              GL_STATIC_DRAW);

  // // Vertex positions
  // glEnableVertexAttribArray(0);
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  // glBindVertexArray(0);
}

void Renderer::DrawBoundingBox(ColliderCube colliderCube, Shader& shader) {
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBindVertexArray(colliderCube.VAO);
  glDrawElements(GL_TRIANGLES, boundingBoxCubeIndices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::Start() {
  // batchIndexCount = 0;
}

// TODO : This assumes that it is only batch drawing lines
void Renderer::Flush() {
  // uint32_t buffer;
  // glGenBuffers(1, &buffer);
  // glBindBuffer(GL_ARRAY_BUFFER, buffer);
  // glBufferData(GL_ARRAY_BUFFER, batchIndexCount * sizeof(glm::mat4), &modelMatrices[0],
  //              GL_STATIC_DRAW);

  // for (size_t i = 0; i < batchIndexCount; i++) {
  // }
}