#include "font.h"

Font::Font(std::string path)
{
  FT_Error error = FT_Init_FreeType(&library);
  if (error)
  {
    std::cout << "Error initializing freetype" << std::endl;
    return;
  }

  FT_Error fileError = FT_New_Face(library, path.c_str(), 0, &face);
  if (error == FT_Err_Unknown_File_Format)
  {
    std::cout << "Freetype Error: Font file can be read but is not in a supported format"
              << std::endl;
    return;
  }
  else if (error)
  {
    std::cout << "Freetype Error: Font file could not be opened or read, or font file is broken"
              << std::endl;
    return;
  }
  std::cout << "Freetype: Font loaded" << std::endl;

  Font::path = path;
}

Font::~Font() = default;