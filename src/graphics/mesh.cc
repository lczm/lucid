#include "mesh.h"

Mesh::Mesh(std::vector<MeshVertex> vertices,
           std::vector<uint32_t> indices,
           std::vector<MeshTexture> textures) {
  Mesh::vertices = vertices;
  Mesh::indices = indices;
  Mesh::textures = textures;

  SetupMesh();
}

void Mesh::SetupMesh() {
  // Generate the appropriate buffers
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // Bind VAO & VBO
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // Set VBO data
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MeshVertex),
               &vertices[0], GL_STATIC_DRAW);

  // Bind EBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  // Set EBO data
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t),
               &indices[0], GL_STATIC_DRAW);

  // Vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
                        (void*)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
                      (void*)offsetof(MeshVertex, normal));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
                        (void*)offsetof(MeshVertex, texCoords));

  // 'unbind' the vertex arrays to not cause future issues
  glBindVertexArray(0);
}

void Mesh::Draw(Shader& shader) {
  uint32_t diffuseNr = 1;
  uint32_t specularNr = 1;

  for (size_t i = 0; i < textures.size(); i++) {
    // Activate texture unit before binding
    glActiveTexture(GL_TEXTURE0 + i);

    // Retrieve texture number
    std::string number;
    std::string name = textures[i].type;

    if (name == "texture_diffuse") number = std::to_string(diffuseNr++);
    else if (name == "texture_specular") number = std::to_string(specularNr++);

    glBindTexture(GL_TEXTURE_2D, textures[i].id);
    shader.SetUniformInt1(("material." + name + number).c_str(), i);
  }

  // Draw the mesh
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  // Reset gl texture unit to 0
  glActiveTexture(GL_TEXTURE0);
}