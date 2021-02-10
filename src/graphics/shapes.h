#pragma once

#include <vector>

#include "shader.h"
#include "glm.hpp"

// Serialization
#include "cereal/archives/json.hpp"

// Vertices
struct LineVertex
{
  glm::vec3 color;
  glm::mat4 modelMatrix;
};

struct SphereVertex
{
  glm::vec3 color;
  glm::mat4 modelMatrix;
};

struct CubeVertex
{
  glm::vec3 color;
  glm::mat4 modelMatrix;
};

// Cubes
const std::vector<float> cubeVertices = {
    // front
    -1.0, -1.0, 1.0,  //
    1.0, -1.0, 1.0,   //
    1.0, 1.0, 1.0,    //
    -1.0, 1.0, 1.0,   //
    // back
    -1.0, -1.0, -1.0,  //
    1.0, -1.0, -1.0,   //
    1.0, 1.0, -1.0,    //
    -1.0, 1.0, -1.0,   //
};

const std::vector<uint32_t> cubeIndices = {
    // front
    0, 1, 2,  //
    2, 3, 0,  //
    // right
    1, 5, 6,  //
    6, 2, 1,  //
    // back
    7, 6, 5,  //
    5, 4, 7,  //
    // left
    4, 0, 3,  //
    3, 7, 4,  //
    // bottom
    4, 5, 1,  //
    1, 0, 4,  //
    // top
    3, 2, 6,  //
    6, 7, 3,  //
};

const std::vector<float> line_vertices = {
    0.0f, 0.0f, 0.0f,  //
    1.0f, 1.0f, 1.0f,  //
};

class Cube
{
 public:
  // Default color to grey
  glm::vec3 color = {0.5f, 0.5f, 0.5f};

 public:
  Cube() = default;
  ~Cube() = default;

  template <class Archive>
  void serialize(Archive& archive)
  {
    archive(                                   //
        cereal::make_nvp("color.r", color.r),  //
        cereal::make_nvp("color.g", color.g),  //
        cereal::make_nvp("color.b", color.b)   //
    );
  }
};

// Spheres
class Sphere
{
 public:
  // Default color to grey
  glm::vec3 color = {0.5f, 0.5f, 0.5f};

 public:
  Sphere() = default;
  ~Sphere() = default;

  template <class Archive>
  void serialize(Archive& archive)
  {
    archive(                                   //
        cereal::make_nvp("color.r", color.r),  //
        cereal::make_nvp("color.g", color.g),  //
        cereal::make_nvp("color.b", color.b)   //
    );
  }
};

class Line
{
 public:
  glm::vec3 origin = glm::vec3(0.0f);
  glm::vec3 destination = glm::vec3(0.0f);

  // Default color to grey
  glm::vec3 color = {0.5f, 0.5f, 0.5f};

 public:
  Line() = default;
  ~Line() = default;
};
