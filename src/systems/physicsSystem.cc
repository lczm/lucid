#include "physicsSystem.h"

PhysicsSystem::PhysicsSystem()
{
}

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::Update(float dt, Registry* registry, Input* input)
{
  // Note to uncomment this in the future
  UpdateAllRigidbodies(dt, registry, input);
  UpdateCollisions(dt, registry, input);
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
  // Try to check collisions between cubes only
  // tcc = {T}ransform-{C}ollider{C}ube
  std::vector<void*> tccComponents = registry->GetComponents<Transform, ColliderCube>();
  auto* tccTransformComponents = static_cast<ComponentVector<Transform>*>(tccComponents[0]);
  auto* tccColliderCubeComponents = static_cast<ComponentVector<ColliderCube>*>(tccComponents[1]);

  // tcs = {T}ransform-{C}ollider{S}phere
  std::vector<void*> tcsComponents = registry->GetComponents<Transform, ColliderSphere>();
  auto* tcsTransformComponents = static_cast<ComponentVector<Transform>*>(tcsComponents[0]);
  auto* tcsColliderSphereComponents =
      static_cast<ComponentVector<ColliderSphere>*>(tcsComponents[1]);

  // tcs = {T}ransform-{C}ollider{P}olygon
  std::vector<void*> tcpComponents = registry->GetComponents<Transform, ColliderPolygon>();
  auto* tcpTransformComponents = static_cast<ComponentVector<Transform>*>(tcpComponents[0]);
  auto* tcpColliderPolygonComponents =
      static_cast<ComponentVector<ColliderPolygon>*>(tcpComponents[1]);

  // TODO : cache i, j pairs so that checking i, j means checking j, i

  {  // Sphere - ... Collisions
    for (size_t i = 0; i < tccColliderCubeComponents->Size(); i++)
    {
      for (size_t j = 0; j < tccColliderCubeComponents->Size(); j++)
      {
        // If they are the same, dont check for collisions
        if (i == j) continue;

        bool collided =
            CheckCollision(tccColliderCubeComponents->At(i), tccTransformComponents->At(i),
                           tccColliderCubeComponents->At(j), tccTransformComponents->At(j));

        if (collided)
        {
          std::cout << "(Cube - Cube) : Collided!" << std::endl;
          tccColliderCubeComponents->At(i).collided = true;
          tccColliderCubeComponents->At(j).collided = true;
        }
      }

      for (size_t j = 0; j < tcsColliderSphereComponents->Size(); j++)
      {
        bool collided =
            CheckCollision(tccColliderCubeComponents->At(i), tccTransformComponents->At(i),
                           tcsColliderSphereComponents->At(j), tcsTransformComponents->At(j));

        if (collided)
        {
          std::cout << "(Cube - Sphere) : Collided!" << std::endl;
          tccColliderCubeComponents->At(i).collided = true;
          tcsColliderSphereComponents->At(i).collided = true;
        }
      }

      for (size_t j = 0; j < tcpColliderPolygonComponents->Size(); j++)
      {
        bool collided =
            CheckCollision(tccColliderCubeComponents->At(i), tccTransformComponents->At(i),
                           tcpColliderPolygonComponents->At(j), tcpTransformComponents->At(j));

        if (collided)
        {
          std::cout << "(Cube - Polygon) : Collided!" << std::endl;
          tccColliderCubeComponents->At(i).collided = true;
          tcpColliderPolygonComponents->At(i).collided = true;
        }
      }
    }
  }

  {  // Sphere - ... Collisions
    for (size_t i = 0; i < tcsColliderSphereComponents->Size(); i++)
    {
      for (size_t j = 0; j < tccColliderCubeComponents->Size(); j++)
      {
        bool collided =
            CheckCollision(tcsColliderSphereComponents->At(i), tcsTransformComponents->At(i),
                           tccColliderCubeComponents->At(j), tccTransformComponents->At(j));

        if (collided)
        {
          std::cout << "(Cube - Sphere) : Collided!" << std::endl;
          tcsColliderSphereComponents->At(i).collided = true;
          tccColliderCubeComponents->At(j).collided = true;
        }
      }

      for (size_t j = 0; j < tcsColliderSphereComponents->Size(); j++)
      {
        if (i == j) continue;

        bool collided =
            CheckCollision(tcsColliderSphereComponents->At(i), tcsTransformComponents->At(i),
                           tcsColliderSphereComponents->At(j), tcsTransformComponents->At(j));

        if (collided)
        {
          std::cout << "(Sphere - Sphere) : Collided!" << std::endl;
          tcsColliderSphereComponents->At(i).collided = true;
          tcsColliderSphereComponents->At(j).collided = true;
        }
      }

      for (size_t j = 0; j < tcpColliderPolygonComponents->Size(); j++)
      {
        bool collided =
            CheckCollision(tcsColliderSphereComponents->At(i), tcsTransformComponents->At(i),
                           tcpColliderPolygonComponents->At(j), tcpTransformComponents->At(j));

        if (collided)
        {
          std::cout << "(Sphere - Polygon) : Collided!" << std::endl;
          tcsColliderSphereComponents->At(i).collided = true;
          tcpColliderPolygonComponents->At(j).collided = true;
        }
      }
    }
  }

  {  // Polygon - ... Collisions
    for (size_t i = 0; i < tcpColliderPolygonComponents->Size(); i++)
    {
      for (size_t j = 0; tccColliderCubeComponents->Size(); j++)
      {
        bool collided =
            CheckCollision(tcpColliderPolygonComponents->At(i), tcpTransformComponents->At(i),
                           tccColliderCubeComponents->At(j), tccTransformComponents->At(j));

        if (collided)
        {
          std::cout << "(Cube - Polygon) : Collided!" << std::endl;
          tcpColliderPolygonComponents->At(i).collided = true;
          tccColliderCubeComponents->At(j).collided = true;
        }
      }

      for (size_t j = 0; j < tcsColliderSphereComponents->Size(); j++)
      {
        bool collided =
            CheckCollision(tcpColliderPolygonComponents->At(i), tcpTransformComponents->At(i),
                           tcsColliderSphereComponents->At(j), tcsTransformComponents->At(j));

        if (collided)
        {
          std::cout << "(Sphere - Polygon) : Collided!" << std::endl;
          tcpColliderPolygonComponents->At(i).collided = true;
          tcsColliderSphereComponents->At(j).collided = true;
        }
      }

      for (size_t j = 0; j < tcsColliderSphereComponents->Size(); j++)
      {
        if (i == j) continue;

        bool collided =
            CheckCollision(tcpColliderPolygonComponents->At(i), tcpTransformComponents->At(i),
                           tcpColliderPolygonComponents->At(j), tcpTransformComponents->At(j));

        if (collided)
        {
          std::cout << "(Polygon- Polygon) : Collided!" << std::endl;
          tcpColliderPolygonComponents->At(i).collided = true;
          tcpColliderPolygonComponents->At(j).collided = true;
        }
      }
    }
  }

  // delete tcsTransformComponents;
  // delete tccColliderCubeComponents;

  // delete tcsTransformComponents;
  // delete tcsColliderSphereComponents;

  // delete tcpTransformComponents;
  // delete tcpColliderPolygonComponents;
}

bool PhysicsSystem::CheckCollision(Collider& colliderA, Transform transformA, Collider& colliderB,
                                   Transform transformB)
{
  std::vector<glm::vec3> vertices;
  glm::vec3 direction = glm::vec3(0.0f);

  Intersection result = Intersection::Unknown;
  while (result == Intersection::Unknown)
  {
    result = EvolveSimplex(vertices, direction, colliderA, transformA, colliderB, transformB);
  }

  bool intersected = result == Intersection::Found;
  return intersected;
}

Intersection PhysicsSystem::EvolveSimplex(std::vector<glm::vec3>& vertices, glm::vec3 direction,
                                          Collider& colliderA, Transform transformA,
                                          Collider& colliderB, Transform transformB)
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
          return Intersection::Found;
        }
      }
    }

    return AddSupport(vertices, direction, colliderA, transformA, colliderB, transformB)
               ? Intersection::Unknown
               : Intersection::None;
  }
}

bool PhysicsSystem::AddSupport(std::vector<glm::vec3>& vertices, glm::vec3 direction,
                               Collider& colliderA, Transform transformA, Collider& colliderB,
                               Transform transformB)
{
  glm::vec3 newVertex = CalculateSupport(direction, colliderA, transformA, colliderB, transformB);
  vertices.push_back(newVertex);
  return glm::dot(direction, newVertex) >= 0;
}

glm::vec3 PhysicsSystem::CalculateSupport(glm::vec3 direction, Collider& colliderA,
                                          Transform transformA, Collider& colliderB,
                                          Transform transformB)
{
  glm::vec3 oppositeDirection = direction;
  oppositeDirection *= -1;

  glm::vec3 newVertex =
      colliderA.Support(transformA, direction) - colliderB.Support(transformB, oppositeDirection);
  return newVertex;
}

bool PhysicsSystem::CheckCollisionBetweenBoundingBox(const BoundingBox boundingBox,
                                                     const BoundingBox boundingBoxOther)
{
  return (boundingBox.minX <= boundingBoxOther.maxX && boundingBox.maxX >= boundingBoxOther.minX) &&
         (boundingBox.minY <= boundingBoxOther.maxY && boundingBox.maxY >= boundingBoxOther.minY) &&
         (boundingBox.minZ <= boundingBoxOther.maxZ && boundingBox.maxZ >= boundingBoxOther.minZ);
}
