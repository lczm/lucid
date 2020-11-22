#include "physicsSystem.h"

PhysicsSystem::PhysicsSystem() {
}

PhysicsSystem::~PhysicsSystem() {
}

void PhysicsSystem::Update(double dt, Registry* registry, Input* input) {
  // Debug Start
  std::vector<void*> components = registry->GetComponents<Transform, RigidBody, BoundingBoxCube>();

  auto* transformComponents = static_cast<ComponentVector<Transform>*>(components[0]);
  auto* rigidBodyComponents = static_cast<ComponentVector<RigidBody>*>(components[1]);
  auto* boundingBoxCubeComponents = static_cast<ComponentVector<BoundingBoxCube>*>(components[2]);

  // If there are less than two components, can ignore checking for collisions
  if (boundingBoxCubeComponents->Size() < 2) {
    return;
  }

  std::unordered_map<uint32_t, bool> collidedCache;

  // Debug :: hard-coded for sphere collisions
  for (size_t i = 0; i < boundingBoxCubeComponents->Size(); i++) {
    for (size_t j = 0; j < boundingBoxCubeComponents->Size(); j++) {
      // TODO can cache results,
      // i.e. if A collides with B, there is no need to check if
      // B collides with A
      if (i == j) continue;

      BoundingBoxCube& collider1 = boundingBoxCubeComponents->At(i);
      Transform& transform1 = transformComponents->At(i);

      BoundingBoxCube& collider2 = boundingBoxCubeComponents->At(j);
      Transform& transform2 = transformComponents->At(j);

      if (collidedCache[i] || collidedCache[j]) continue;

      if (CheckCollision(collider1, transform1, collider2, transform2)) {
        collider1.color = glm::vec3(1.0f, 0.0f, 0.0f);
        collider2.color = glm::vec3(1.0f, 0.0f, 0.0f);

        collidedCache[i] = true;
        collidedCache[j] = true;
      } else {
        collider1.color = glm::vec3(1.0f, 1.0f, 1.0f);
        collider2.color = glm::vec3(1.0f, 1.0f, 1.0f);

        collidedCache[i] = false;
        collidedCache[j] = false;
      }
    }
  }
  // Debug End
}

bool PhysicsSystem::CheckCollision(BoundingBoxCube& boundingBoxCube, Transform& transform,
                                   BoundingBoxCube& boundingBoxCubeOther,
                                   Transform& transformOther) {
  // Assume that it is a box/cube...
  // Get both the model transforms
  glm::mat4 boundingBoxModelMatrix = ApplyTransformation(transform);
  glm::mat4 boundingBoxModelMatrixOther = ApplyTransformation(transformOther);

  std::vector<glm::vec4> bbVertices;
  std::vector<glm::vec4> bbVerticesOther;

  for (size_t i = 0; i < boundingBoxCubeVertices.size(); i += 3) {
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

  for (size_t i = 0; i < bbVertices.size(); i++) {
    bb.minX = glm::min(bbVertices[i].x, bb.minX);
    bb.maxX = glm::max(bbVertices[i].x, bb.maxX);
    bbOther.minX = glm::min(bbVerticesOther[i].x, bbOther.minX);
    bbOther.maxX = glm::max(bbVerticesOther[i].x, bbOther.maxX);

    bb.minY = glm::min(bbVertices[i].y, bb.minY);
    bb.maxY = glm::max(bbVertices[i].y, bb.maxY);
    bbOther.minY = glm::min(bbVerticesOther[i].y, bbOther.minY);
    bbOther.maxY = glm::max(bbVerticesOther[i].y, bbOther.maxY);

    bb.minZ = glm::min(bbVertices[i].z, bb.minZ);
    bb.maxZ = glm::max(bbVertices[i].z, bb.maxZ);
    bbOther.minZ = glm::min(bbVerticesOther[i].z, bbOther.minZ);
    bbOther.maxZ = glm::max(bbVerticesOther[i].z, bbOther.maxZ);
  }
  if (CheckCollisionBetweenBoundingBox(bb, bbOther)) {
    return true;
  }
}

glm::mat4 PhysicsSystem::ApplyTransformation(Transform& transform) {
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
                                                     const BoundingBox boundingBoxOther) {
  return (boundingBox.minX <= boundingBoxOther.maxX && boundingBox.maxX >= boundingBoxOther.minX) &&
         (boundingBox.minY <= boundingBoxOther.maxY && boundingBox.maxY >= boundingBoxOther.minY) &&
         (boundingBox.minZ <= boundingBoxOther.maxZ && boundingBox.maxZ >= boundingBoxOther.minZ);
}

void PhysicsSystem::GetAxisAlignedBoundingBox(ColliderSphere& collider, Transform& transform) {
}

void PhysicsSystem::GetOrientedBoundingBox(ColliderSphere& collider, Transform& transform) {
}
