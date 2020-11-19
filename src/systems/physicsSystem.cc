#include "physicsSystem.h"

PhysicsSystem::PhysicsSystem() {
}

PhysicsSystem::~PhysicsSystem() {
}

void PhysicsSystem::Update(double dt, Registry* registry, Input* input) {
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
  // TODO : This cant be done, overload the boundingbox or find an alternative method
  // matrixModel *= boundingBox;

  rigidBody.aabb = boundingBox;
}