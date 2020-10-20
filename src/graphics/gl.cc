#include "gl.h"

// -- Shaders
Shader::Shader() {
}

Shader::~Shader() {
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::readFile(std::string& path, std::string& output) {
  std::ifstream inFile;
  std::stringstream inStream;

  inFile.open(path);
  inStream << inFile.rdbuf();
  output = inStream.str();
}

void Shader::bind() {
  glUseProgram(id);
}

void Shader::unbind() {
  glUseProgram(0);
}

void Shader::createShader(std::string vertexPath, std::string fragmentPath) {
  readFile(vertexPath, vertexSource);
  readFile(fragmentPath, fragmentSource);

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

void Shader::setUniformInt1(const std::string& name, int v) {
  glUniform1i(glGetUniformLocation(id, &name[0]), v);
}

void Shader::setUniformFloat1(const std::string& name, float v) {
  glUniform1f(glGetUniformLocation(id, &name[0]), v);
}

void Shader::setUniformInt2(const std::string& name, int v1, int v2) {
  glUniform2i(glGetUniformLocation(id, &name[0]), v1, v2);
}

void Shader::setUniformFloat2(const std::string& name, float v1, float v2) {
  glUniform2f(glGetUniformLocation(id, &name[0]), v1, v2);
}

void Shader::setUniformInt3(const std::string& name, int v1, int v2, int v3) {
  glUniform3i(glGetUniformLocation(id, &name[0]), v1, v2, v3);
}

void Shader::setUniformFloat3(const std::string& name, float v1, float v2,
                              float v3) {
  glUniform3f(glGetUniformLocation(id, &name[0]), v1, v2, v3);
}

void Shader::setUniformInt4(const std::string& name, int v1, int v2, int v3,
                            int v4) {
  glUniform4i(glGetUniformLocation(id, &name[0]), v1, v2, v3, v4);
}

void Shader::setUniformFloat4(const std::string& name, float v1, float v2,
                              float v3, float v4) {
  glUniform4f(glGetUniformLocation(id, &name[0]), v1, v2, v3, v4);
}

void Shader::setUniformMatFloat4(const std::string& name, glm::mat4 mat) {
  // The false here is to indicate that there is no need to transpose the matrix
  glUniformMatrix4fv(glGetUniformLocation(id, &name[0]), 1, false, &mat[0][0]);
}

// -- VertexBufferObjects (VBOs)
VertexBuffer::VertexBuffer() {
}

VertexBuffer::~VertexBuffer() {
  glDeleteBuffers(1, &id);
}

void VertexBuffer::bind() {
  glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::unbind() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::bindData(const void* data, unsigned int size) {
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

// -- IndexBuffers
IndexBuffer::IndexBuffer() {
}

IndexBuffer::~IndexBuffer() {
  glDeleteBuffers(1, &id);
}

void IndexBuffer::bind() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBuffer::unbind() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::bindData(const unsigned int* data, unsigned int count) {
  IndexBuffer::count = count;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data,
               GL_STATIC_DRAW);
}

unsigned int IndexBuffer::getCount() {
  return IndexBuffer::count;
}

unsigned int IndexBuffer::getId() {
  return IndexBuffer::id;
}

// VertexBufferLayout
VertexBufferLayout::VertexBufferLayout() {
  VertexBufferLayout::stride = 0;
}

VertexBufferLayout::~VertexBufferLayout() {
}

void VertexBufferLayout::push(unsigned int count) {
  elements.push_back({GL_FLOAT, count, GL_FALSE});
  stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
}

// VertexArrays
VertexArray::VertexArray() {
}

VertexArray::~VertexArray() {
  glDeleteVertexArrays(1, &id);
}

void VertexArray::bind() {
  glBindVertexArray(id);
}

void VertexArray::unbind() {
  glBindVertexArray(0);
}

void VertexArray::addBuffer(VertexBuffer& vb, VertexBufferLayout& layout) {
  bind();
  vb.bind();

  const auto& elements = layout.getElements();
  unsigned int offset = 0;
  for (unsigned int i = 0; i < elements.size(); i++) {
    const auto& element = elements[i];
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(
        i, element.count, element.type, element.normalized, layout.getStride(),
        (void*)(size_t)offset);  // Ensures pointer size when cast
    offset += element.count * VertexBufferElement::getSizeOfType(element.type);
  }
}

// Texture
Texture::Texture() {
}

Texture::Texture(const char imagePath[], int slot) {
  glGenTextures(1, &id);

  stbi_set_flip_vertically_on_load(1);
  data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
  if (!data) {
    std::cout << "Data not loaded, something went wrong" << std::endl;
    std::cout << "Image Path : " << imagePath << std::endl;
  }

  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, id);

  // These have to be loaded in
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  // These are set to GL_NEAREST instead of GL_LINEAR, because this is
  // pixel game and nearest would be very clear instead of blurred
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture() {
  glDeleteTextures(1, &id);
  stbi_image_free(data);
}

void Texture::initialize(const char imagePath[], int slot) {
  stbi_set_flip_vertically_on_load(1);
  data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
  if (!data) {
    std::cout << "Data not loaded, something went wrong" << std::endl;
    std::cout << "Image Path : " << imagePath << std::endl;
  }

  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, id);

  // These have to be loaded in
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  // These are set to GL_NEAREST instead of GL_LINEAR, because this is
  // pixel game and nearest would be very clear instead of blurred
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::bind() {
  glBindTexture(GL_TEXTURE_2D, id);
}
void Texture::unbind() {
  glBindTexture(GL_TEXTURE_2D, 0);
}
