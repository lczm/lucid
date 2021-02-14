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

 public:
  float scale;
  glm::vec3 color;
  std::string text;

 public:
  Font();
  Font(std::string path, std::string text, float scale, glm::vec3 color = glm::vec3(255, 255, 255));
  ~Font();
  void SetPixelSize(FT_F26Dot6 charHeight, FT_F26Dot6 charWidth);
  void DrawText(std::string text, float x, float y, float scale,
                glm::vec3 color = glm::vec3(255, 255, 255));
  void DrawText(float x, float y);
  void SetColor(glm::vec3 color);
  void SetScale(float scale);

 private:
  void LoadCharacters();
};