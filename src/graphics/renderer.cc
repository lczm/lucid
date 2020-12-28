#include "renderer.h"

Renderer::Renderer(Registry* registry)
    : linePrimitiveBuffer(MAX_BUFFER),
      cubePrimititiveBuffer(MAX_BUFFER),
      spherePrimitiveBuffer(MAX_BUFFER)
{
  Renderer::registry = registry;
  Renderer::stats = &registry->GetComponent<RendererStats>();
};

Renderer::~Renderer() = default;

void Renderer::DrawMesh(Mesh& mesh, Shader& shader)
{
  shader.Bind();
  uint32_t diffuseNr = 1;
  uint32_t specularNr = 1;

  for (size_t i = 0; i < mesh.textures.size(); i++)
  {
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
  // glDrawElementsBaseVertex(GL_TRIANGLES, mesh.numIndices, GL_UNSIGNED_INT,
  //                          (void*)(sizeof(uint32_t) * mesh.baseIndex), mesh.baseVertex);

  glBindVertexArray(0);

  // Reset gl texture unit to 0
  glActiveTexture(GL_TEXTURE0);

  // Update stats
  stats->calls++;
  stats->vertices += 1 * mesh.vertices.size();
  stats->indices += mesh.indices.size();
}

void Renderer::DrawModel(Model& model, Shader& shader)
{
  for (Mesh& mesh : model.GetMeshes())
  {
    DrawMesh(mesh, shader);
  }
}

void Renderer::DrawCube(Cube& cube, Shader& shader)
{
  glBindVertexArray(cube.VAO);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube.EBO);
  glDrawElements(GL_TRIANGLES, cubeIndices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  // Update stats
  stats->calls++;
  stats->vertices += 1 * cubeVertices.size();
  stats->indices += cubeIndices.size();
}

void Renderer::DrawCubeIndexed(const PrimitiveBatchIds primitiveBatchIds)
{
  glBindVertexArray(primitiveBatchIds.cubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, primitiveBatchIds.cubeVBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, primitiveBatchIds.cubeEBO);

  uint32_t instanceBuffer;
  glGenBuffers(1, &instanceBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
  glBufferData(GL_ARRAY_BUFFER, MAX_BUFFER * sizeof(CubeVertex), &cubePrimititiveBuffer[0],
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);
  glEnableVertexAttribArray(4);
  glEnableVertexAttribArray(5);

  std::size_t vec4Size = sizeof(glm::vec4);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (4 * vec4Size) + (3 * sizeof(float)), (void*)0);

  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, (4 * vec4Size) + (3 * sizeof(float)),
                        (void*)(3 * sizeof(float)));

  glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, (4 * vec4Size) + (3 * sizeof(float)),
                        (void*)(3 * sizeof(float) + (1 * vec4Size)));

  glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, (4 * vec4Size) + (3 * sizeof(float)),
                        (void*)(3 * sizeof(float) + (2 * vec4Size)));

  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, (4 * vec4Size) + (3 * sizeof(float)),
                        (void*)(3 * sizeof(float) + (3 * vec4Size)));

  glVertexAttribDivisor(1, 1);  // colors
  glVertexAttribDivisor(2, 1);  // mat4 : vec1
  glVertexAttribDivisor(3, 1);  // mat4 : vec2
  glVertexAttribDivisor(4, 1);  // mat4 : vec3
  glVertexAttribDivisor(5, 1);  // mat4 : vec4

  glDrawElementsInstanced(GL_TRIANGLES, cubeIndices.size(), GL_UNSIGNED_INT, 0, batchIndexCount);

  glBindVertexArray(0);
  glDeleteBuffers(1, &instanceBuffer);

  // Update stats
  stats->calls++;
  stats->vertices += batchIndexCount * cubeVertices.size();
  stats->indices += cubeIndices.size();
}

void Renderer::DrawSphere(Sphere& sphere, Shader& shader)
{
  /*
  glBindVertexArray(sphere.VAO);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere.EBO);
  glDrawElements(GL_TRIANGLES, sphere.indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  */
}

void Renderer::DrawSphereIndexed(const PrimitiveBatchIds primitiveBatchIds)
{
  glBindVertexArray(primitiveBatchIds.sphereVAO);
  glBindBuffer(GL_ARRAY_BUFFER, primitiveBatchIds.sphereVBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, primitiveBatchIds.sphereEBO);

  uint32_t instanceBuffer;
  glGenBuffers(1, &instanceBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
  glBufferData(GL_ARRAY_BUFFER, MAX_BUFFER * sizeof(SphereVertex), &spherePrimitiveBuffer[0],
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);
  glEnableVertexAttribArray(4);
  glEnableVertexAttribArray(5);

  std::size_t vec4Size = sizeof(glm::vec4);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (4 * vec4Size) + (3 * sizeof(float)), (void*)0);

  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, (4 * vec4Size) + (3 * sizeof(float)),
                        (void*)(3 * sizeof(float)));

  glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, (4 * vec4Size) + (3 * sizeof(float)),
                        (void*)(3 * sizeof(float) + (1 * vec4Size)));

  glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, (4 * vec4Size) + (3 * sizeof(float)),
                        (void*)(3 * sizeof(float) + (2 * vec4Size)));

  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, (4 * vec4Size) + (3 * sizeof(float)),
                        (void*)(3 * sizeof(float) + (3 * vec4Size)));

  glVertexAttribDivisor(1, 1);  // colors
  glVertexAttribDivisor(2, 1);  // mat4 : vec1
  glVertexAttribDivisor(3, 1);  // mat4 : vec2
  glVertexAttribDivisor(4, 1);  // mat4 : vec3
  glVertexAttribDivisor(5, 1);  // mat4 : vec4

  SphereVerticesIndices sphereVerticesIndices = registry->GetComponent<SphereVerticesIndices>();

  glDrawElementsInstanced(GL_TRIANGLES, sphereVerticesIndices.indices.size(), GL_UNSIGNED_INT, 0,
                          batchIndexCount);

  glBindVertexArray(0);
  glDeleteBuffers(1, &instanceBuffer);

  // Update stats
  stats->calls++;
  stats->vertices += batchIndexCount * sphereVerticesIndices.vertices.size();
  stats->indices += sphereVerticesIndices.indices.size();
}

// void Renderer::DrawLine(Line& line, Shader& shader)
// {
//   glBindVertexArray(line.VAO);
//   glDrawArrays(GL_LINES, 0, 2);
//   glBindVertexArray(0);
// }

void Renderer::DrawLineIndexed(const PrimitiveBatchIds primitiveBatchIds)
{
  glBindVertexArray(primitiveBatchIds.lineVAO);
  glBindBuffer(GL_ARRAY_BUFFER, primitiveBatchIds.lineVBO);

  glBufferSubData(GL_ARRAY_BUFFER, 0, batchIndexCount * sizeof(LineVertex),
                  &linePrimitiveBuffer[0]);

  glDrawArraysInstanced(GL_LINES, 0, 2, batchIndexCount);

  glBindVertexArray(0);

  // Update stats
  stats->calls++;
  stats->vertices += batchIndexCount * 2;
  // There are no indices in lines
}

void Renderer::DrawBoundingBox(const BoundingBox bb)
{
  Transform transform;
  glm::mat4 lineModelMatrix;

  // Draw top surface
  {
    transform.position = glm::vec3(bb.minX, bb.maxY, bb.minZ);
    transform.scale = glm::vec3(bb.minX, bb.maxY, bb.maxZ) - glm::vec3(bb.minX, bb.maxY, bb.minZ);
    lineModelMatrix = GetModelMatrix(transform);
    PushGenericBufferWithColor(lineModelMatrix, {1.0f, 0.0f, 0.0f}, DrawType::Line);

    transform.position = glm::vec3(bb.maxX, bb.maxY, bb.minZ);
    transform.scale = glm::vec3(bb.maxX, bb.maxY, bb.maxZ) - glm::vec3(bb.maxX, bb.maxY, bb.minZ);
    lineModelMatrix = GetModelMatrix(transform);
    PushGenericBufferWithColor(lineModelMatrix, {1.0f, 0.0f, 0.0f}, DrawType::Line);

    transform.position = glm::vec3(bb.minX, bb.maxY, bb.minZ);
    transform.scale = glm::vec3(bb.maxX, bb.maxY, bb.minZ) - glm::vec3(bb.minX, bb.maxY, bb.minZ);
    lineModelMatrix = GetModelMatrix(transform);
    PushGenericBufferWithColor(lineModelMatrix, {1.0f, 0.0f, 0.0f}, DrawType::Line);

    transform.position = glm::vec3(bb.minX, bb.maxY, bb.maxZ);
    transform.scale = glm::vec3(bb.maxX, bb.maxY, bb.maxZ) - glm::vec3(bb.minX, bb.maxY, bb.maxZ);
    lineModelMatrix = GetModelMatrix(transform);
    PushGenericBufferWithColor(lineModelMatrix, {1.0f, 0.0f, 0.0f}, DrawType::Line);
  }

  // Draw bottom surface
  {
    transform.position = glm::vec3(bb.minX, bb.minY, bb.minZ);
    transform.scale = glm::vec3(bb.minX, bb.minY, bb.maxZ) - glm::vec3(bb.minX, bb.minY, bb.minZ);
    lineModelMatrix = GetModelMatrix(transform);
    PushGenericBufferWithColor(lineModelMatrix, {1.0f, 0.0f, 0.0f}, DrawType::Line);

    transform.position = glm::vec3(bb.maxX, bb.minY, bb.minZ);
    transform.scale = glm::vec3(bb.maxX, bb.minY, bb.maxZ) - glm::vec3(bb.maxX, bb.minY, bb.minZ);
    lineModelMatrix = GetModelMatrix(transform);
    PushGenericBufferWithColor(lineModelMatrix, {1.0f, 0.0f, 0.0f}, DrawType::Line);

    transform.position = glm::vec3(bb.minX, bb.minY, bb.minZ);
    transform.scale = glm::vec3(bb.maxX, bb.minY, bb.minZ) - glm::vec3(bb.minX, bb.minY, bb.minZ);
    lineModelMatrix = GetModelMatrix(transform);
    PushGenericBufferWithColor(lineModelMatrix, {1.0f, 0.0f, 0.0f}, DrawType::Line);

    transform.position = glm::vec3(bb.minX, bb.minY, bb.maxZ);
    transform.scale = glm::vec3(bb.maxX, bb.minY, bb.maxZ) - glm::vec3(bb.minX, bb.minY, bb.maxZ);
    lineModelMatrix = GetModelMatrix(transform);
    PushGenericBufferWithColor(lineModelMatrix, {1.0f, 0.0f, 0.0f}, DrawType::Line);
  }

  // Draw top-bottom, bottom-top lines
  {
    transform.position = glm::vec3(bb.minX, bb.minY, bb.maxZ);
    transform.scale = glm::vec3(bb.minX, bb.maxY, bb.maxZ) - glm::vec3(bb.minX, bb.minY, bb.maxZ);
    lineModelMatrix = GetModelMatrix(transform);
    PushGenericBufferWithColor(lineModelMatrix, {1.0f, 0.0f, 0.0f}, DrawType::Line);

    transform.position = glm::vec3(bb.maxX, bb.minY, bb.maxZ);
    transform.scale = glm::vec3(bb.maxX, bb.maxY, bb.maxZ) - glm::vec3(bb.maxX, bb.minY, bb.maxZ);
    lineModelMatrix = GetModelMatrix(transform);
    PushGenericBufferWithColor(lineModelMatrix, {1.0f, 0.0f, 0.0f}, DrawType::Line);

    transform.position = glm::vec3(bb.minX, bb.minY, bb.minZ);
    transform.scale = glm::vec3(bb.minX, bb.maxY, bb.minZ) - glm::vec3(bb.minX, bb.minY, bb.minZ);
    lineModelMatrix = GetModelMatrix(transform);
    PushGenericBufferWithColor(lineModelMatrix, {1.0f, 0.0f, 0.0f}, DrawType::Line);

    transform.position = glm::vec3(bb.maxX, bb.minY, bb.minZ);
    transform.scale = glm::vec3(bb.maxX, bb.maxY, bb.minZ) - glm::vec3(bb.maxX, bb.minY, bb.minZ);
    lineModelMatrix = GetModelMatrix(transform);
    PushGenericBufferWithColor(lineModelMatrix, {1.0f, 0.0f, 0.0f}, DrawType::Line);
  }
}

void Renderer::StartBatch()
{
  // Reset the batch counter
  batchIndexCount = 0;
}

void Renderer::ClearRendererStats()
{
  stats->calls = 0;
  stats->vertices = 0;
  stats->indices = 0;
}

void Renderer::PushLineBuffer(const glm::mat4 modelMatrix, const Line line)
{
  // If there is too much to add on, flush it down before adding
  if (batchIndexCount == MAX_BUFFER)
  {
    PrimitiveBatchIds primitiveBatchIds = registry->GetComponent<PrimitiveBatchIds>();
    FlushBatch(primitiveBatchIds, DrawType::Line);
  }

  linePrimitiveBuffer[batchIndexCount].color = line.color;
  linePrimitiveBuffer[batchIndexCount].modelMatrix = modelMatrix;
  batchIndexCount++;
}

void Renderer::PushSphereBuffer(const glm::mat4 modelMatrix, const Sphere sphere)
{
  // If there is too much to add on, flush it down before adding
  if (batchIndexCount == MAX_BUFFER)
  {
    PrimitiveBatchIds primitiveBatchIds = registry->GetComponent<PrimitiveBatchIds>();
    FlushBatch(primitiveBatchIds, DrawType::Sphere);
  }

  spherePrimitiveBuffer[batchIndexCount].color = sphere.color;
  spherePrimitiveBuffer[batchIndexCount].modelMatrix = modelMatrix;
  batchIndexCount++;
}

void Renderer::PushCubeBuffer(const glm::mat4 modelMatrix, const Cube cube)
{
  // If there is too much to add on, flush it down before adding
  if (batchIndexCount == MAX_BUFFER)
  {
    PrimitiveBatchIds primitiveBatchIds = registry->GetComponent<PrimitiveBatchIds>();
    FlushBatch(primitiveBatchIds, DrawType::Cube);
  }

  cubePrimititiveBuffer[batchIndexCount].color = cube.color;
  cubePrimititiveBuffer[batchIndexCount].modelMatrix = modelMatrix;
  batchIndexCount++;
}

void Renderer::PushModelBuffer(const glm::mat4 modelMatrix, const Model model)
{
}

void Renderer::PushGenericBufferWithColor(const glm::mat4 modelMatrix, const glm::vec3 color,
                                          const DrawType drawType)
{
  PrimitiveBatchIds primitiveBatchIds = registry->GetComponent<PrimitiveBatchIds>();
  switch (drawType)
  {
    case DrawType::Line:
      if (batchIndexCount == MAX_BUFFER)
      {
        FlushBatch(primitiveBatchIds, DrawType::Line);
      }

      linePrimitiveBuffer[batchIndexCount].color = color;
      linePrimitiveBuffer[batchIndexCount].modelMatrix = modelMatrix;
      batchIndexCount++;
      break;
    case DrawType::Cube:
      if (batchIndexCount == MAX_BUFFER)
      {
        FlushBatch(primitiveBatchIds, DrawType::Cube);
      }

      cubePrimititiveBuffer[batchIndexCount].color = color;
      cubePrimititiveBuffer[batchIndexCount].modelMatrix = modelMatrix;
      batchIndexCount++;
      break;
    case DrawType::Sphere:
      if (batchIndexCount == MAX_BUFFER)
      {
        FlushBatch(primitiveBatchIds, DrawType::Sphere);
      }

      spherePrimitiveBuffer[batchIndexCount].color = color;
      spherePrimitiveBuffer[batchIndexCount].modelMatrix = modelMatrix;
      batchIndexCount++;
      break;
  }
}

// TODO : This assumes its all lines
void Renderer::FlushBatch(PrimitiveBatchIds primitiveBatchIds, const DrawType drawType)
{
  // Nothing to draw
  if (batchIndexCount == 0)
  {
    return;
  }

  switch (drawType)
  {
    case DrawType::Line:
      DrawLineIndexed(primitiveBatchIds);
      break;
    case DrawType::Sphere:
      DrawSphereIndexed(primitiveBatchIds);
      break;
    case DrawType::Cube:
      DrawCubeIndexed(primitiveBatchIds);
      break;
    case DrawType::Model:
      break;
  }
  batchIndexCount = 0;
}