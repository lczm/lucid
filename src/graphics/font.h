#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <string>

class Font
{
 private:
  FT_Library library;
  FT_Face face;
  std::string path;

 public:
  Font(std::string path);
  ~Font();
};