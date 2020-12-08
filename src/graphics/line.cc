#include "line.h"

Line::Line() {
  /*
  // Generate the appropriate buffers
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  // Bind VAO & VBO
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // Set VBO data
  glBufferData(GL_ARRAY_BUFFER, line_vertices.size() * sizeof(float), &line_vertices[0],
               GL_STATIC_DRAW);

  // Vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  glBindVertexArray(0);
  */
}

Line::~Line() {
}

DevLine::DevLine() {
  // Generate the appropriate buffers
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  // Bind VAO & VBO
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // Set VBO data
  glBufferData(GL_ARRAY_BUFFER, line_vertices.size() * sizeof(float), &line_vertices[0],
               GL_STATIC_DRAW);

  // Vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  glBindVertexArray(0);
}

DevLine::~DevLine() {
}