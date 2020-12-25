#include "model.h"

Model::Model(){};

Model::Model(std::string path)
{
  LoadModel(path);
}

void Model::LoadModel(std::string path)
{
  Assimp::Importer importer;
  scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    std::cout << "Error : " << importer.GetErrorString() << std::endl;
    return;
  }

  directory = path.substr(0, path.find_last_of('/'));
  ProcessNode(scene->mRootNode, scene);

  // CalculateModelBoundingBox();
}

std::vector<Mesh> Model::GetMeshes()
{
  return Model::meshes;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
  // Process all node meshes
  for (size_t i = 0; i < node->mNumMeshes; i++)
  {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(ProcessMesh(mesh, scene));
  }

  // Recursively process each of its children
  for (size_t i = 0; i < node->mNumChildren; i++)
  {
    ProcessNode(node->mChildren[i], scene);
  }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
  std::vector<MeshVertex> vertices;
  std::vector<uint32_t> indices;
  std::vector<MeshTexture> textures;

  for (size_t i = 0; i < mesh->mNumVertices; i++)
  {
    MeshVertex vertex;

    // Process vertex positions, normals, texture coordinates
    // glm::vec3 position;
    // position.x = mesh->mVertices[i].x;
    // position.y = mesh->mVertices[i].y;
    // position.z = mesh->mVertices[i].z;

    // vertex.position = position;
    vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
    vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

    if (mesh->mTextureCoords[0])
    {
      vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
    }
    else
    {
      vertex.texCoords = glm::vec2(0.0f, 0.0f);
    }

    vertices.push_back(vertex);
  }

  // Process indices
  for (size_t i = 0; i < mesh->mNumFaces; i++)
  {
    aiFace face = mesh->mFaces[i];
    for (size_t j = 0; j < face.mNumIndices; j++)
    {
      indices.push_back(face.mIndices[j]);
    }
  }

  // Process material
  if (mesh->mMaterialIndex >= 0)
  {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<MeshTexture> diffuseMaps =
        LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<MeshTexture> specularMaps =
        LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  }

  return Mesh(vertices, indices, textures, scene);
}

std::vector<MeshTexture> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type,
                                                     std::string typeName)
{
  std::vector<MeshTexture> textures;

  for (size_t i = 0; i < material->GetTextureCount(type); i++)
  {
    aiString str;
    material->GetTexture(type, i, &str);

    bool skip = false;
    for (size_t j = 0; j < loadedTextures.size(); j++)
    {
      if (std::strcmp(loadedTextures[j].path.data(), str.C_Str()) == 0)
      {
        textures.push_back(loadedTextures[j]);
        skip = true;  // a texture with the same filepath has already been
                      // loaded, continue to next one. (optimization)
        break;
      }
    }

    if (!skip)
    {
      MeshTexture texture;
      texture.id = TextureFromFile(str.C_Str(), directory, false);
      texture.type = typeName;
      texture.path = str.C_Str();
      textures.push_back(texture);
      loadedTextures.push_back(texture);
    }
  }
  return textures;
}

uint32_t Model::TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
  std::string filename = std::string(path);
  filename = directory + '/' + filename;

  uint32_t textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

  if (data)
  {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  }
  else
  {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}

// void Model::CalculateModelBoundingBox() {
//   BoundingBox boundingBox;
//
//   for (size_t i = 0; i < meshes.size(); i++) {
//     std::vector<MeshVertex>& vertices = meshes[i].vertices;
//     for (size_t j = 0; j < vertices.size(); j++) {
//       boundingBox.minX = glm::min(boundingBox.minX, vertices[j].position.x);
//       boundingBox.maxX = glm::max(boundingBox.maxX, vertices[j].position.x);
//
//       boundingBox.minY = glm::min(boundingBox.minY, vertices[j].position.y);
//       boundingBox.maxY = glm::max(boundingBox.maxY, vertices[j].position.y);
//
//       boundingBox.minZ = glm::min(boundingBox.minZ, vertices[j].position.z);
//       boundingBox.maxZ = glm::max(boundingBox.maxZ, vertices[j].position.z);
//     }
//   }
//
//   Model::boundingBox = boundingBox;
// }