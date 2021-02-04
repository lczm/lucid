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
  if (fileError == FT_Err_Unknown_File_Format)
  {
    std::cout << "Freetype Error: Font file can be read but is not in a supported format"
              << std::endl;
    return;
  }
  else if (fileError)
  {
    std::cout << "Freetype Error: Font file could not be opened or read, or font file is broken"
              << std::endl;
    return;
  }

  FT_Error charSizeError = FT_Set_Char_Size(face, 0, 0, 1440, 900);

  if (charSizeError)
  {
    std::cout << "Freetype Error: Could not set character size" << std::endl;
  }

  std::cout << "Freetype: Font loaded" << std::endl;

  Font::path = path;
}

Font::~Font() = default;