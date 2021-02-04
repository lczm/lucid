#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <string>
#include <map>
#include "glm.hpp"
#include "shader.h"

struct Character
{
  unsigned int textureID;
  glm::ivec2 size;
  glm::ivec2 bearing;
  unsigned int advance;
};

class Font
{
 private:
  FT_Library library;
  FT_Face face;
  std::map<char, Character> characters;
  std::string path;
  unsigned int VAO, VBO;
  Shader shader;

 public:
  Font(std::string path);
  ~Font();
  void SetPixelSize(FT_F26Dot6 charHeight, FT_F26Dot6 charWidth);
  void DrawText(std::string text, float x, float y, float scale,
                glm::vec3 color = glm::vec3(255, 255, 255));

 private:
  void LoadCharacters();
};