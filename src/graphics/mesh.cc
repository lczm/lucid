#include "mesh.h"

Mesh::Mesh(std::vector<MeshVertex> vertices, std::vector<uint32_t> indices,
           std::vector<MeshTexture> textures, std::vector<VertexBoneData> bones,
           const aiScene* scene, uint32_t meshNumIndices, uint32_t meshBaseVertex,
           uint32_t meshBaseIndex)
{
  Mesh::vertices = vertices;
  Mesh::indices = indices;
  Mesh::textures = textures;
  Mesh::bones = bones;

  Mesh::scene = scene;

  Mesh::numIndices = meshNumIndices;
  Mesh::baseVertex = meshBaseVertex;
  Mesh::baseIndex = meshBaseIndex;

  SetupMesh();
}

void Mesh::SetupMesh()
{
  // Generate the appropriate buffers
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &boneVBO);
  glGenBuffers(1, &EBO);

  // Bind VAO & VBO
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // Set VBO data
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MeshVertex), &vertices[0], GL_STATIC_DRAW);

  // Bind EBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  // Set EBO data
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0],
               GL_STATIC_DRAW);

  // Vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void*)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
                        (void*)offsetof(MeshVertex, normal));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
                        (void*)offsetof(MeshVertex, texCoords));

  // Setup bone vertex attribs
  glBindBuffer(GL_ARRAY_BUFFER, boneVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(bones[0]) * bones.size(), &bones[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(3);
  glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (void*)0);

  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (void*)16);

  // 'unbind' the vertex arrays to not cause future issues
  glBindVertexArray(0);
}
