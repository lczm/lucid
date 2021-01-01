#include "colliders.h"

ColliderCube::ColliderCube()
{
}

ColliderCube::~ColliderCube()
{
}

glm::vec3 ColliderCube::Support(Transform& transform, glm::vec3 direction)
{
  return glm::vec3(0.0f);
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

  return center + radius * glm::normalize(direction);
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
  return glm::vec3(0.0f);
}