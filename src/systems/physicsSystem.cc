#include "physicsSystem.h"

PhysicsSystem::PhysicsSystem() {
}

PhysicsSystem::~PhysicsSystem() {
}

void PhysicsSystem::Update(double dt, Registry* registry, Input* input) {
  // Debug Start
  std::vector<void*> components = registry->GetComponents<Transform, RigidBody, ColliderSphere>();

  auto* transformComponents = static_cast<ComponentVector<Transform>*>(components[0]);
  auto* rigidBodyComponents = static_cast<ComponentVector<RigidBody>*>(components[1]);
  auto* colliderComponents = static_cast<ComponentVector<ColliderSphere>*>(components[2]);

  // If there are less than two components, can ignore checking for collisions
  if (colliderComponents->Size() < 2) {
    return;
  }

  // Debug :: hard-coded for sphere collisions
  for (size_t i = 0; i < colliderComponents->Size(); i++) {
    for (size_t j = 0; j < colliderComponents->Size(); j++) {
      if (i == j) continue;

      ColliderSphere& collider1 = colliderComponents->At(i);
      Transform& transform1 = transformComponents->At(i);

      ColliderSphere& collider2 = colliderComponents->At(j);
      Transform& transform2 = transformComponents->At(j);

      CollisionCheck(collider1, transform1, collider2, transform2);
    }
  }
  // Debug End
}

void PhysicsSystem::CollisionCheck(ColliderSphere& collider, Transform& transform,
                                   ColliderSphere& colliderOther, Transform& transformOther) {
  // std::cout << "Checking for collision start" << std::endl;
  // std::cout << "Checking for collision end" << std::endl;
}

void PhysicsSystem::GetAxisAlignedBoundingBox(ColliderSphere& collider, Transform& transform) {
}

void PhysicsSystem::GetOrientedBoundingBox(ColliderSphere& collider, Transform& transform) {
}

void PhysicsSystem::CalculateBoundingBox(Cube& const cube, Transform& const transform,
                                         RigidBody& rigidBody) {
}

void PhysicsSystem::CalculateBoundingBox(Sphere& const sphere, Transform& const transform,
                                         RigidBody& rigidBody) {
}

// Note that this computation is quite expensive and should be done as little as possible
// Specifically for model objects
void PhysicsSystem::CalculateBoundingBox(Model& const model, Transform& const transform,
                                         RigidBody& rigidBody) {
  BoundingBox boundingBox;
  auto meshes = model.GetMeshes();

  glm::mat4 matrixModel = glm::mat4(1.0f);
  glm::mat4 rotationMatrix = glm::mat4(1.0f);

  matrixModel = glm::translate(matrixModel, transform.position);
  matrixModel = glm::scale(matrixModel, transform.scale);

  // Rotation matrix
  rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[0], glm::vec3(1.0, 0.0, 0.0));
  rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[1], glm::vec3(0.0, 1.0, 0.0));
  rotationMatrix = glm::rotate(rotationMatrix, transform.rotation[2], glm::vec3(0.0, 0.0, 1.0));

  // Note : not too sure if this is correct, check by visualizing
  matrixModel *= rotationMatrix;

  // Note that this should be initialized once only
  for (size_t i = 0; i < meshes.size(); i++) {
    std::vector<MeshVertex>& vertices = meshes[i].vertices;
    for (size_t j = 0; j < vertices.size(); j++) {
      boundingBox.minX = glm::min(boundingBox.minX, vertices[j].position.x);
      boundingBox.maxX = glm::max(boundingBox.maxX, vertices[j].position.x);

      boundingBox.minY = glm::min(boundingBox.minY, vertices[j].position.y);
      boundingBox.maxY = glm::max(boundingBox.maxY, vertices[j].position.y);

      boundingBox.minZ = glm::min(boundingBox.minZ, vertices[j].position.z);
      boundingBox.maxZ = glm::max(boundingBox.maxZ, vertices[j].position.z);
    }
  }

  // TODO : This cant be done, overload the boundingbox or find an alternative method

  // matrixModel *= boundingBox;
  // rigidBody.aabb = boundingBox;
}