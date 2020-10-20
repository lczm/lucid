#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

std::string readFile(std::string path) {
  std::ifstream inFile;
  inFile.open(path);

  std::stringstream stringStream;
  stringStream << inFile.rdbuf();
  std::string str = stringStream.str();

  return str;
}

unsigned int compileShader(unsigned int type, std::string& source) {
  // Create a char pointer to the first index of the string
  char* src = &source[0];
  unsigned int shaderId = glCreateShader(type);

  // Last parameter indicates length, can set it to null
  glShaderSource(shaderId, 1, &src, NULL);
  glCompileShader(shaderId);

  // TODO: Error checking before to make sure shader is compiled
  return shaderId;
}
