#pragma once
#include <iostream>
#include <glad/gl.h>

void GlClearError()
{
  while (glGetError() != GL_NO_ERROR)
    ;
}

bool GlCheckError()
{
  while (GLenum error = glGetError())
  {
    std::cout << "GL Error : " << std::hex << error << std::endl;
    return true;
  }
  return false;
}
