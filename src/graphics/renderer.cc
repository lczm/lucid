#include "renderer.h"

Renderer::Renderer(Registry* registry) : linePrimitiveBuffer(MAX_BUFFER)
{
  Renderer::registry = registry;
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
  glBindVertexArray(0);

  // Reset gl texture unit to 0
  glActiveTexture(GL_TEXTURE0);
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
}

void Renderer::DrawSphere(Sphere& sphere, Shader& shader)
{
  glBindVertexArray(sphere.VAO);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere.EBO);
  glDrawElements(GL_TRIANGLES, sphere.indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Renderer::DrawLine(Line& line, Shader& shader)
{
  glBindVertexArray(line.VAO);
  glDrawArrays(GL_LINES, 0, 2);
  glBindVertexArray(0);
}

void Renderer::DrawLineIndexed(PrimitiveBatchIds primitiveBatchIds)
{
  glBindVertexArray(primitiveBatchIds.lineVAO);
  glBindBuffer(GL_ARRAY_BUFFER, primitiveBatchIds.lineVBO);

  glBufferSubData(GL_ARRAY_BUFFER, 0, batchIndexCount * sizeof(LineVertex),
                  &linePrimitiveBuffer[0]);

  glDrawArraysInstanced(GL_LINES, 0, 2, batchIndexCount);

  glBindVertexArray(0);
}

void Renderer::DrawBoundingBox(Model& model, Shader& shader)
{
}

void Renderer::DrawBoundingBox(Sphere& sphere, Shader& shader)
{
}

void Renderer::DrawBoundingBox(ColliderCube colliderCube, Shader& shader)
{
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBindVertexArray(colliderCube.VAO);
  glDrawElements(GL_TRIANGLES, boundingBoxCubeIndices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::StartBatch()
{
  // Reset the batch counter
  batchIndexCount = 0;
}

void Renderer::PushLineBuffer(glm::mat4 modelMatrix, Line line)
{
  // If there is too much to add on, flush it down before adding
  if (batchIndexCount == MAX_BUFFER)
  {
    PrimitiveBatchIds& primitiveBatchIds = registry->GetComponent<PrimitiveBatchIds>();
    FlushBatch(primitiveBatchIds, DrawType::Line);
  }
  linePrimitiveBuffer[batchIndexCount].color = line.color;
  linePrimitiveBuffer[batchIndexCount].modelMatrix = modelMatrix;
  batchIndexCount++;
}

// TODO : This assumes its all lines
void Renderer::FlushBatch(PrimitiveBatchIds primitiveBatchIds, DrawType drawType)
{
  switch (drawType)
  {
    case DrawType::Line:
      DrawLineIndexed(primitiveBatchIds);
      break;
    case DrawType::Sphere:
      break;
    case DrawType::Cube:
      break;
    case DrawType::Model:
      break;
  }
  batchIndexCount = 0;
}