#include "colliders.h"

ColliderCube::ColliderCube()
{
}

ColliderCube::~ColliderCube()
{
}

// TODO : Take in vec3 or vec4 direction?
glm::vec3 ColliderCube::Support(Transform& transform, glm::vec3 direction)
{
  auto modelMatrix = GetModelMatrix(transform, ColliderCube::transform);
  std::vector<glm::vec4> vertices = GetCubeVertices(modelMatrix);

  glm::vec4 maxVertex = glm::vec4(1.0f);
  float maxDistance = -std::numeric_limits<float>::infinity();

  for (glm::vec4 vertex : vertices)
  {
    float distance = glm::dot(vertex, glm::vec4(direction, 1.0f));
    if (distance > maxDistance)
    {
      maxDistance = distance;
      maxVertex = vertex;
    }
  }

  // TODO : return vec3 or vec4?
  // return maxVertex;
  return glm::vec3(maxVertex.x, maxVertex.y, maxVertex.z);
}

ColliderSphere::ColliderSphere()
{
}

glm::vec3 ColliderSphere::Support(Transform& transform, glm::vec3 direction)
{
  // Center of a collider sphere is transform.position
  glm::vec3 center = transform.position;
  // Standardized, TODO : Whether this needs to be scaled or not
  float radius = 1.0f;

  return glm::vec3(0.0f);
  // return center + radius * glm::normalize(direction);
}

ColliderSphere::~ColliderSphere()
{
}

ColliderPolygon::ColliderPolygon()
{
}

ColliderPolygon::~ColliderPolygon()
{
}

void ColliderPolygon::SetVertices(std::vector<glm::vec4> vertices)
{
  ColliderPolygon::vertices = vertices;
}

glm::vec3 ColliderPolygon::Support(Transform& transform, glm::vec3 direction)
{
  auto modelMatrix = GetModelMatrix(transform, ColliderPolygon::transform);

  glm::vec4 maxVertex = glm::vec4(1.0f);
  float maxDistance = -std::numeric_limits<float>::infinity();

  for (glm::vec4 vertex : vertices)
  {
    // Get the updated vertex with the just transformed modelMatrix
    // Note that `*=` cannot be used here, as that would be parsed
    // into a = a * b which is not what vector multiplication is trying
    // to acheive
    vertex = modelMatrix * vertex;

    float distance = glm::dot(vertex, glm::vec4(direction, 1.0f));
    if (distance > maxDistance)
    {
      maxDistance = distance;
      maxVertex = vertex;
    }
  }

  // TODO : return vec3 or vec4?
  // return maxVertex;
  return glm::vec3(maxVertex.x, maxVertex.y, maxVertex.z);
}