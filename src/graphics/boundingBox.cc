#include "boundingBox.h"

BoundingBoxCube::BoundingBoxCube() {
}

BoundingBoxCube::~BoundingBoxCube() {
}

void BoundingBoxCube::Build() {
  // Generate the appropriate buffers
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // Bind VAO & VBO
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // Set VBO data
  glBufferData(GL_ARRAY_BUFFER, boundingBoxCubeVertices.size() * sizeof(float),
               &boundingBoxCubeVertices[0], GL_STATIC_DRAW);

  // Bind EBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  // Set EBO data
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, boundingBoxCubeIndices.size() * sizeof(uint32_t),
               &boundingBoxCubeIndices[0], GL_STATIC_DRAW);

  // Vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  // glEnableVertexAttribArray(1);
  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

  glBindVertexArray(0);
}