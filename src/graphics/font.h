#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>

class Font
{
 private:
  FT_Library library;

 public:
  Font();
  ~Font();
};