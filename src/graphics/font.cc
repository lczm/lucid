#include "font.h"

Font::Font()
{
}

Font::Font(std::string path, std::string text, float scale, glm::vec3 color)
{
  Font::scale = scale;
  Font::color = color;
  Font::text = text;

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

  LoadCharacters();

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // destroy freetype once done
  FT_Done_Face(face);
  FT_Done_FreeType(library);
}

Font::~Font() = default;

void Font::SetPixelSize(FT_F26Dot6 charHeight, FT_F26Dot6 charWidth)
{
  FT_Error charSizeError = FT_Set_Char_Size(face, charHeight, charWidth, 1440, 900);

  if (charSizeError)
  {
    std::cout << "Freetype Error: Could not set character size" << std::endl;
  }

  std::cout << "Freetype: Font loaded" << std::endl;
}

// Color defaulted to white
void Font::DrawText(std::string text, float x, float y, float scale, glm::vec3 color)
{
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(VAO);

  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++)
  {
    Character ch = characters[*c];

    float xPos = x + ch.bearing.x * scale;
    float yPos = y - (ch.size.y - ch.bearing.y) * scale;

    float w = ch.size.x * scale;
    float h = ch.size.y * scale;

    float vertices[6][4] = {{xPos, yPos + h, 0.0f, 0.0f}, {xPos, yPos, 0.0f, 1.0f},
                            {xPos + w, yPos, 1.0f, 1.0f}, {xPos, yPos + h, 0.0f, 0.0f},
                            {xPos + w, yPos, 1.0f, 1.0f}, {xPos + w, yPos + h, 1.0f, 0.0f}};

    // render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.textureID);

    // update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // advance cursors for next glyph
    x += (ch.advance >> 6) * scale;
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Font::DrawText(float x, float y)
{
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(VAO);

  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++)
  {
    Character ch = characters[*c];

    float xPos = x + ch.bearing.x * scale;
    float yPos = y - (ch.size.y - ch.bearing.y) * scale;

    float w = ch.size.x * scale;
    float h = ch.size.y * scale;

    float vertices[6][4] = {{xPos, yPos + h, 0.0f, 0.0f}, {xPos, yPos, 0.0f, 1.0f},
                            {xPos + w, yPos, 1.0f, 1.0f}, {xPos, yPos + h, 0.0f, 0.0f},
                            {xPos + w, yPos, 1.0f, 1.0f}, {xPos + w, yPos + h, 1.0f, 0.0f}};

    // render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.textureID);

    // update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // advance cursors for next glyph
    x += (ch.advance >> 6) * scale;
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Font::SetColor(glm::vec3 color)
{
  Font::color = color;
}

void Font::SetScale(float scale)
{
  Font::scale = scale;
}

void Font::LoadCharacters()
{
  FT_Set_Pixel_Sizes(face, 0, 48);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  // First 128 characters of the ASCII character set
  for (unsigned char c = 0; c < 128; c++)
  {
    // load character glyph
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
      std::cout << "Freetype Error : Failed to load glyph " + c << std::endl;
      continue;
    }

    // generate texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0,
                 GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // store characters for later use
    Character character = {texture, glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                           glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                           face->glyph->advance.x};
    characters.insert(std::pair<char, Character>(c, character));
  }
  glBindTexture(GL_TEXTURE_2D, 0);
}
