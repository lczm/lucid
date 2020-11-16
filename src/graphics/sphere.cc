#include "sphere.h"

Sphere::Sphere() {
}

Sphere::Sphere(float radius, int sectors, int stacks) {
  Sphere::radius = radius;
  Sphere::sectors = sectors;
  Sphere::stacks = stacks;
}

Sphere::~Sphere() = default;

void Sphere::Build() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // Bind VAO & VBO
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // Set VBO data
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

  // Bind EBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  // Set EBO data
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0],
               GL_STATIC_DRAW);

  // Vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

  // glEnableVertexAttribArray(2);
  // glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)6);

  glBindVertexArray(0);
}

void Sphere::BuildSphere() {
  // vertex positions
  float x, y, z, xy;

  // vertex normals
  float nx, ny, nz;
  float lengthInverse = 1.0f / radius;
  float s, t;

  float sectorStep = 2 * PI / sectors;
  float stackStep = PI / stacks;
  float sectorAngle, stackAngle;

  for (int i = 0; i <= stacks; i++) {
    stackAngle = PI / 2 - i * stackStep;

    xy = radius * cosf(stackAngle);
    z = radius * sinf(stackAngle);

    // add (sectorCount+1) vertices per stack
    // the first and last vertices have same position and normal, but different texCoord
    for (int j = 0; j <= sectors; j++) {
      sectorAngle = j * sectorStep;

      // vertex position (x, y, z)
      x = xy * cosf(sectorAngle);
      y = xy * sinf(sectorAngle);
      vertices.push_back(x);
      vertices.push_back(y);
      vertices.push_back(z);

      // Note : This can be uncommented out in the future, Setting this to rgb colours for
      // simplicity
      // normalized vertex normal (nx, ny, nz)
      nx = x * lengthInverse;
      ny = y * lengthInverse;
      nz = z * lengthInverse;
      normals.push_back(nx);
      normals.push_back(ny);
      normals.push_back(nz);

      // Note : This can be uncommented out in the future, Setting this to rgb colours for
      // simplicity
      // vertex tex coord (s, t) range between [0, 1]
      s = (float)j / sectors;
      t = (float)i / stacks;

      texCoords.push_back(s);
      texCoords.push_back(t);

      // RGB colours
      vertices.push_back(0.5f);
      vertices.push_back(0.5f);
      vertices.push_back(0.5f);
    }
  }

  int k1, k2;
  for (int i = 0; i < stacks; ++i) {
    k1 = i * (sectors + 1);  // beginning of current stack
    k2 = k1 + sectors + 1;   // beginning of next stack

    for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
      // 2 triangles per sector excluding first and last stacks
      // k1 => k2 => k1+1
      if (i != 0) {
        indices.push_back(k1);
        indices.push_back(k2);
        indices.push_back(k1 + 1);
      }

      // k1+1 => k2 => k2+1
      if (i != (stacks - 1)) {
        indices.push_back(k1 + 1);
        indices.push_back(k2);
        indices.push_back(k2 + 1);
      }
    }
  }
}
