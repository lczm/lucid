#include "shader.h"

// -- Shaders
Shader::Shader()
{
}

Shader::~Shader()
{
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::ReadFile(std::string& path, std::string& output)
{
  std::ifstream inFile;
  std::stringstream inStream;

  inFile.open(path);
  inStream << inFile.rdbuf();
  output = inStream.str();
}

void Shader::Bind()
{
  glUseProgram(id);
}

void Shader::Unbind()
{
  glUseProgram(0);
}

void Shader::CreateShader(std::string vertexPath, std::string fragmentPath)
{
  ReadFile(vertexPath, vertexSource);
  ReadFile(fragmentPath, fragmentSource);

  vSrc = &vertexSource[0];
  fSrc = &fragmentSource[0];

  // Compile the shaders
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vSrc, NULL);
  glCompileShader(vertexShader);

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fSrc, NULL);
  glCompileShader(fragmentShader);

  // Create a shader program and link the vertex and fragment shaders
  id = glCreateProgram();
  glAttachShader(id, vertexShader);
  glAttachShader(id, fragmentShader);

  // glBindFragDataLocation(id, 0, "outColor");

  glLinkProgram(id);
  // glUseProgram(id);
}

void Shader::SetUniformInt1(const std::string& name, int v)
{
  glUniform1i(glGetUniformLocation(id, &name[0]), v);
}

void Shader::SetUniformFloat1(const std::string& name, float v)
{
  glUniform1f(glGetUniformLocation(id, &name[0]), v);
}

void Shader::SetUniformInt2(const std::string& name, int v1, int v2)
{
  glUniform2i(glGetUniformLocation(id, &name[0]), v1, v2);
}

void Shader::SetUniformFloat2(const std::string& name, float v1, float v2)
{
  glUniform2f(glGetUniformLocation(id, &name[0]), v1, v2);
}

void Shader::SetUniformInt3(const std::string& name, int v1, int v2, int v3)
{
  glUniform3i(glGetUniformLocation(id, &name[0]), v1, v2, v3);
}

void Shader::SetUniformFloat3(const std::string& name, float v1, float v2, float v3)
{
  glUniform3f(glGetUniformLocation(id, &name[0]), v1, v2, v3);
}

void Shader::SetUniformInt4(const std::string& name, int v1, int v2, int v3, int v4)
{
  glUniform4i(glGetUniformLocation(id, &name[0]), v1, v2, v3, v4);
}

void Shader::SetUniformFloat4(const std::string& name, float v1, float v2, float v3, float v4)
{
  glUniform4f(glGetUniformLocation(id, &name[0]), v1, v2, v3, v4);
}

void Shader::SetUniformVecFloat3(const std::string& name, glm::vec3 v3)
{
  glUniform3fv(glGetUniformLocation(id, &name[0]), 1, &v3[0]);
}

void Shader::SetUniformMatFloat4(const std::string& name, glm::mat4 mat)
{
  // The false here is to indicate that there is no need to transpose the matrix
  glUniformMatrix4fv(glGetUniformLocation(id, &name[0]), 1, false, &mat[0][0]);
}

void Shader::SetUniformMatFloat4(const std::string& name, uint32_t count,
                                 std::vector<glm::mat4> mats)
{
  glUniformMatrix4fv(glGetUniformLocation(id, &name[0]), count, GL_FALSE, glm::value_ptr(mats[0]));
}