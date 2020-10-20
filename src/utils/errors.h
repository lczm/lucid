#pragma once
#include <iostream>
#include <glad/gl.h>

void glClearError() {
  while (glGetError() != GL_NO_ERROR)
    ;
}

bool glCheckError() {
  while (GLenum error = glGetError()) {
    std::cout << "GL Error : " << std::hex << error << std::endl;
    return true;
  }
  return false;
}
