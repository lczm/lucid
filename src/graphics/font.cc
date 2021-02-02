#include "font.h"

Font::Font()
{
  FT_Error error = FT_Init_FreeType(&library);
  if (error)
  {
    std::cout << "Error initializing freetype" << std::endl;
  }
}