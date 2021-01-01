#include "physicsSystem.h"

PhysicsSystem::PhysicsSystem()
{
}

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::Update(float dt, Registry* registry, Input* input)
{
  // UpdateAllRigidbodies(dt, registry, input);
  // UpdateCollisions(dt, registry, input);
  Test(dt, registry, input);
}

void PhysicsSystem::UpdateAllRigidbodies(float dt, Registry* registry, Input* input)
{
  RigidBodyConfiguration& rigidBodyConfiguration = registry->GetResource<RigidBodyConfiguration>();

  registry->GetComponentsIter<Transform, RigidBody>()->Each(
      [&](Transform& transform, RigidBody& rigidBody) {
        transform.position += rigidBody.velocity;

        // If the rigidbody component has applyGravity, then apply the gravity.
        if (rigidBody.applyGravity)
        {
          transform.position.y -= rigidBodyConfiguration.gravityRate;
        }
      });
}

void PhysicsSystem::UpdateCollisions(float dt, Registry* registry, Input* input)
{
  std::vector<void*> components = registry->GetComponents<Transform, RigidBody, ColliderCube>();

  auto* transformComponents = static_cast<ComponentVector<Transform>*>(components[0]);
  auto* rigidBodyComponents = static_cast<ComponentVector<RigidBody>*>(components[1]);
  auto* colliderCubeComponents = static_cast<ComponentVector<ColliderCube>*>(components[2]);

  // If there are less than two components, can ignore checking for collisions
  if (colliderCubeComponents->Size() < 2)
  {
    return;
  }

  std::unordered_map<uint32_t, bool> collidedCache;

  for (size_t i = 0; i < colliderCubeComponents->Size(); i++)
  {
    for (size_t j = 0; j < colliderCubeComponents->Size(); j++)
    {
      // TODO can cache results,
      // i.e. if A collides with B, there is no need to check if
      // B collides with A
      if (i == j) continue;

      ColliderCube& collider1 = colliderCubeComponents->At(i);
      Transform& transform1 = transformComponents->At(i);

      ColliderCube& collider2 = colliderCubeComponents->At(j);
      Transform& transform2 = transformComponents->At(j);

      if (collidedCache[i] || collidedCache[j]) continue;

      if (CheckCollision(collider1, transform1, collider2, transform2))
      {
        collider1.color = glm::vec3(1.0f, 0.0f, 0.0f);
        collider2.color = glm::vec3(1.0f, 0.0f, 0.0f);

        collidedCache[i] = true;
        collidedCache[j] = true;
        collider1.collided = true;
        collider2.collided = true;
      }
      else
      {
        collider1.color = glm::vec3(1.0f, 1.0f, 1.0f);
        collider2.color = glm::vec3(1.0f, 1.0f, 1.0f);

        collidedCache[i] = false;
        collidedCache[j] = false;
        collider1.collided = false;
        collider2.collided = false;
      }
    }
  }

  delete transformComponents;
  delete rigidBodyComponents;
  delete colliderCubeComponents;
}

void PhysicsSystem::Test(float dt, Registry* registry, Input* input)
{
  // std::cout << "Inside PhysicsSystem::Test()" << std::endl;
  // Try to check collisions between cubes only
  std::vector<void*> components = registry->GetComponents<Transform, RigidBody, ColliderCube>();

  auto* transformComponents = static_cast<ComponentVector<Transform>*>(components[0]);
  auto* colliderCubeComponents = static_cast<ComponentVector<ColliderCube>*>(components[2]);

  // Start to test collision
  vertices.clear();
  colliderA = &(colliderCubeComponents->At(0));
  colliderB = &(colliderCubeComponents->At(1));
  transformA = &(transformComponents->At(0));
  transformB = &(transformComponents->At(1));

  EvolveResult result = EvolveResult::StillEvolving;
  while (result == EvolveResult::StillEvolving)
  {
    result = EvolveSimplex(*(transformA), *(transformB));
  }

  bool intersected = result == EvolveResult::FoundIntersection;
  if (intersected)
  {
    std::cout << "Cube intersected" << std::endl;
  }
}

EvolveResult PhysicsSystem::EvolveSimplex(Transform transformA, Transform transformB)
{
  {
    switch (vertices.size())
    {
      case 0:
        direction = transformA.position - transformB.position;
        break;
      case 1:
        // Flip the direction
        direction *= -1;
        break;
      case 2:
      {
        // line formed by the first two vertices
        glm::vec3 ab = vertices[1] - vertices[0];
        // line formed from the first vertex to the origin
        glm::vec3 a0 = vertices[0] *= -1;

        // triple-cross product to calculate a direction perpendicular
        // to line ab in the direction of the origin
        // TODO : need to check if this is correct
        glm::vec3 temp = glm::cross(ab, a0);
        direction = glm::cross(temp, ab);
        break;
      }
      case 3:
      {
        glm::vec3 ac = vertices[2] - vertices[0];
        glm::vec3 ab = vertices[1] - vertices[0];
        direction = glm::cross(ac, ab);

        // ensure that it points towards the origin
        glm::vec3 a0 = vertices[0] *= -1;
        if (glm::dot(direction, a0) < 0)
        {
          direction *= -1;
        }
        break;
      }
      case 4:
      {
        // calculate the three edges of interest
        glm::vec3 da = vertices[3] - vertices[0];
        glm::vec3 db = vertices[3] - vertices[1];
        glm::vec3 dc = vertices[3] - vertices[2];

        // calculate the direction to the origin
        glm::vec3 d0 = vertices[3];
        d0 *= -1;

        // calculate and check triangles a-b-d, b-c-d, c-a-d
        glm::vec3 abdNorm = glm::cross(da, db);
        glm::vec3 bcdNorm = glm::cross(db, dc);
        glm::vec3 cadNorm = glm::cross(dc, da);

        if (glm::dot(abdNorm, d0) > 0)
        {
          // the origin is on the outside of the triangle a-b-d
          // elimicate c
          vertices.erase(vertices.begin() + 2);
          direction = abdNorm;
        }
        else if (glm::dot(bcdNorm, d0) > 0)
        {
          // the origin is on the outside of the triangle b-c-d
          // eliminate a
          vertices.erase(vertices.begin() + 0);
          direction = bcdNorm;
        }
        else if (glm::dot(cadNorm, d0) > 0)
        {
          // the origin is on the outside of the triangle c-a-d
          // eliminate b
          vertices.erase(vertices.begin() + 1);
          direction = cadNorm;
        }
        else
        {
          return EvolveResult::FoundIntersection;
        }
      }
    }
    return AddSupport(direction) ? EvolveResult::StillEvolving : EvolveResult::NoIntersection;
  }
}

glm::vec3 PhysicsSystem::CalculateSupport(glm::vec3 direction)
{
  glm::vec3 oppositeDirection = direction;
  oppositeDirection *= -1;

  glm::vec3 newVertex = colliderA->Support(*(transformA), direction) -
                        colliderB->Support(*(transformB), oppositeDirection);
  return newVertex;
}

bool PhysicsSystem::AddSupport(glm::vec3 direction)
{
  glm::vec3 newVertex = CalculateSupport(direction);
  vertices.push_back(newVertex);
  return glm::dot(direction, newVertex) >= 0;
}

bool PhysicsSystem::CheckCollision(ColliderCube colliderCube, Transform& transform,
                                   ColliderCube colliderCubeOther, Transform& transformOther)
{
  // Assume that it is a box/cube...
  // Get both the model transforms
  glm::mat4 boundingBoxModelMatrix = ApplyTransformation(transform);
  glm::mat4 boundingBoxModelMatrixOther = ApplyTransformation(transformOther);

  std::vector<glm::vec4> bbVertices;
  std::vector<glm::vec4> bbVerticesOther;

  for (size_t i = 0; i < boundingBoxCubeVertices.size(); i += 3)
  {
    bbVertices.push_back(boundingBoxModelMatrix * glm::vec4(boundingBoxCubeVertices[i],
                                                            boundingBoxCubeVertices[i + 1],
                                                            boundingBoxCubeVertices[i + 2], 1.0f));

    bbVerticesOther.push_back(boundingBoxModelMatrixOther *
                              glm::vec4(boundingBoxCubeVertices[i],      //
                                        boundingBoxCubeVertices[i + 1],  //
                                        boundingBoxCubeVertices[i + 2], 1.0f));
  }

  BoundingBox bb;
  BoundingBox bbOther;

  for (size_t i = 0; i < bbVertices.size(); i++)
  {
    // Min-Max x-values
    bb.minX = glm::min(bbVertices[i].x, bb.minX);
    bb.maxX = glm::max(bbVertices[i].x, bb.maxX);
    bbOther.minX = glm::min(bbVerticesOther[i].x, bbOther.minX);
    bbOther.maxX = glm::max(bbVerticesOther[i].x, bbOther.maxX);

    // Min-Max y-values
    bb.minY = glm::min(bbVertices[i].y, bb.minY);
    bb.maxY = glm::max(bbVertices[i].y, bb.maxY);
    bbOther.minY = glm::min(bbVerticesOther[i].y, bbOther.minY);
    bbOther.maxY = glm::max(bbVerticesOther[i].y, bbOther.maxY);

    // Min-Max z-values
    bb.minZ = glm::min(bbVertices[i].z, bb.minZ);
    bb.maxZ = glm::max(bbVertices[i].z, bb.maxZ);
    bbOther.minZ = glm::min(bbVerticesOther[i].z, bbOther.minZ);
    bbOther.maxZ = glm::max(bbVerticesOther[i].z, bbOther.maxZ);
  }

  if (CheckCollisionBetweenBoundingBox(bb, bbOther))
  {
    return true;
  }

  return false;
}

glm::mat4 PhysicsSystem::ApplyTransformation(Transform& transform)
{
  glm::mat4 matrixModel = glm::mat4(1.0f);
  glm::mat4 rotationMatrix = glm::mat4(1.0f);

  matrixModel = glm::translate(matrixModel, transform.position);
  matrixModel = glm::scale(matrixModel, transform.scale);

  // Rotation matrix
  rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[0], glm::vec3(1.0, 0.0, 0.0));
  rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[1], glm::vec3(0.0, 1.0, 0.0));
  rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[2], glm::vec3(0.0, 0.0, 1.0));

  matrixModel *= rotationMatrix;
  return matrixModel;
}

bool PhysicsSystem::CheckCollisionBetweenBoundingBox(const BoundingBox boundingBox,
                                                     const BoundingBox boundingBoxOther)
{
  return (boundingBox.minX <= boundingBoxOther.maxX && boundingBox.maxX >= boundingBoxOther.minX) &&
         (boundingBox.minY <= boundingBoxOther.maxY && boundingBox.maxY >= boundingBoxOther.minY) &&
         (boundingBox.minZ <= boundingBoxOther.maxZ && boundingBox.maxZ >= boundingBoxOther.minZ);
}
