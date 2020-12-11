#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

// Note : ReadFile and CompileShader is not really needed anymore

// std::string ReadFile(std::string path) {
//   std::ifstream inFile;
//   inFile.open(path);
//
//   std::stringstream stringStream;
//   stringStream << inFile.rdbuf();
//   std::string str = stringStream.str();
//
//   return str;
// }

// uint32_t CompileShader(unsigned int type, std::string& source) {
//   // Create a char pointer to the first index of the string
//   char* src = &source[0];
//   unsigned int shaderId = glCreateShader(type);
//
//   // Last parameter indicates length, can set it to null
//   glShaderSource(shaderId, 1, &src, NULL);
//   glCompileShader(shaderId);
//
//   // TODO: Error checking before to make sure shader is compiled
//   return shaderId;
// }

namespace lucid
{
template <typename... Args>
void Log(Args&&... args)
{
  (std::cout << ... << args) << std::endl;
}

}  // namespace lucid