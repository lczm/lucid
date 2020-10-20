#pragma once

#include "constants.h"
#include "gl.h"

/*
   A standard place to store components.
   Components should just be defined as POD (Plain Old Data)
   i.e. simply a struct.

  Note : the structs/components that are declared here should have a default
   value.
*/

struct Transform {
  float x = 0;
  float y = 0;
  float z = 0;
  float scale = 1;
};

struct Animation {
  float animCounter = 0;   // Counter on time passed
  float animInterval = 1;  // Counter on the threshold of each animation

  int spriteCurrent = 0;  // Current sprite
  int spriteStart = 0;
  int spriteEnd = 0;
};

// TODO : This can be refactored into something easier to use
// for opengl
struct Image {
  int spriteX = 0;
  int spriteY = 0;

  // These values are passed into the model matrix for transformation
  int imageSlot = 1;
  // These are used to get which sprite in the sheet is being used
  float sheetWidth = 1;
  float sheetHeight = 1;
  float spriteWidth = 1;
  float spriteHeight = 1;

  float scaledWidth = 1;
  float scaledHeight = 1;

  int totalSpriteRow = 1;
  int totalSpriteCol = 1;

  // These will hold position, color, texcoords
  std::vector<Vertex> vertices;

  glm::mat4 projection;
  glm::mat4 view;

  Texture* texture = new Texture();
  VertexArray* vertexArray = new VertexArray();
  VertexBuffer* vertexBuffer = new VertexBuffer();
  VertexBufferLayout* vertexBufferLayout = new VertexBufferLayout();
  IndexBuffer* indexBuffer = new IndexBuffer();
  Shader* shader = new Shader();
};
