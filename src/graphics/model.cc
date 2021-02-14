#include "model.h"

Model::Model() : boneMatrices(100)
{
}

Model::Model(std::string path, Registry* registry) : boneMatrices(100)
{
  Model::path = path;
  Model::registry = registry;

  importer = new Assimp::Importer();
  scene = importer->ReadFile(
      path, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    std::cout << "(Model Loading) Error : " << importer->GetErrorString() << std::endl;
    return;
  }

  if (scene->mNumAnimations > 0)
  {
    hasAnimations = true;
    for (size_t i = 0; i < scene->mNumAnimations; i++)
    {
      auto animation = scene->mAnimations[i];
      for (size_t j = 0; j < animation->mNumChannels; j++)
      {
        // for every animation, channel add it to the mapping
        auto channel = animation->mChannels[j];
        animationMapping[std::pair<uint32_t, std::string>(i, channel->mNodeName.C_Str())] = j;
      }
      animationIdMapping[animation->mName.C_Str()] = i;
    }
  }

  directory = path.substr(0, path.find_last_of('/'));
  ProcessNode(scene->mRootNode, scene);

  // After processing everything, generate a standalone vertices bounding box
  for (auto& mesh : GetMeshes())
  {
    // Reserve some space in advance
    vertices.reserve(mesh.vertices.size());
    for (size_t i = 0; i < mesh.vertices.size(); i++)
    {
      vertices.push_back(glm::vec4(mesh.vertices[i].position.x, mesh.vertices[i].position.y,
                                   mesh.vertices[i].position.z, 1.0f));
    }
  }

  // Initialize the bounding box
  boundingBox = GetBoundingBox(vertices);
}

Model::~Model()
{
}

void Model::Load()
{
  importer = new Assimp::Importer();
  scene = importer->ReadFile(
      path, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    std::cout << "(Model Loading) Error : " << importer->GetErrorString() << std::endl;
    return;
  }

  if (scene->mNumAnimations > 0)
  {
    hasAnimations = true;
    for (size_t i = 0; i < scene->mNumAnimations; i++)
    {
      auto animation = scene->mAnimations[i];
      for (size_t j = 0; j < animation->mNumChannels; j++)
      {
        // for every animation, channel add it to the mapping
        auto channel = animation->mChannels[j];
        animationMapping[std::pair<uint32_t, std::string>(i, channel->mNodeName.C_Str())] = j;
      }
      animationIdMapping[animation->mName.C_Str()] = i;
    }
  }

  directory = path.substr(0, path.find_last_of('/'));
  ProcessNode(scene->mRootNode, scene);

  // After processing everything, generate a standalone vertices bounding box
  for (auto& mesh : GetMeshes())
  {
    // Reserve some space in advance
    vertices.reserve(mesh.vertices.size());
    for (size_t i = 0; i < mesh.vertices.size(); i++)
    {
      vertices.push_back(glm::vec4(mesh.vertices[i].position.x, mesh.vertices[i].position.y,
                                   mesh.vertices[i].position.z, 1.0f));
    }
  }

  // Initialize the bounding box
  boundingBox = GetBoundingBox(vertices);
}

std::vector<Mesh> Model::GetMeshes()
{
  return Model::meshes;
}

aiNode* Model::GetRootNode()
{
  return scene->mRootNode;
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
  // MeshVertex : {position, normals, texcoords}
  std::vector<MeshVertex> vertices;
  std::vector<uint32_t> indices;
  std::vector<MeshTexture> textures;

  // Populate and process MeshVertex(s)
  for (size_t i = 0; i < mesh->mNumVertices; i++)
  {
    MeshVertex vertex;

    // Process vertex positions, normals, texture coordinates
    // vertex.position = glm::make_vec3(&mesh->mVertices[i].x);
    vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

    if (mesh->mNormals)
    {
      // vertex.normal = glm::make_vec3(&mesh->mNormals[i].x);
      vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
    }
    else
    {
      vertex.normal = glm::vec3(0.0f);
    }

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

  // Populate and process indices
  for (size_t i = 0; i < mesh->mNumFaces; i++)
  {
    aiFace face = mesh->mFaces[i];
    for (size_t j = 0; j < face.mNumIndices; j++)
    {
      indices.push_back(face.mIndices[j]);
    }
  }

  // Populate and process material
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

  // Populate and process bones
  for (size_t i = 0; i < mesh->mNumBones; i++)
  {
    auto bone = mesh->mBones[i];
    uint32_t id = 0;
    if (boneMapping.count(bone->mName.C_Str()))
    {
      id = boneMapping[bone->mName.C_Str()];
    }
    else
    {
      boneMapping[bone->mName.C_Str()] = boneMapping.size() + 1;
      id = boneMapping.size();
    }

    boneOffsets.resize(std::max(id + 1, static_cast<uint32_t>(boneOffsets.size())));
    boneOffsets[id] = CastToGlmMat4(bone->mOffsetMatrix);

    for (size_t j = 0; j < bone->mNumWeights; j++)
    {
      auto weight = bone->mWeights[j];
      vertices[weight.mVertexId].AddBone(id, weight.mWeight);
    }
  }

  return Mesh(vertices, indices, textures, scene);
}

void Model::UpdateBoneMatrices(float dt, uint32_t animationId, aiNode* node, glm::mat4 transform)
{
  auto animation = scene->mAnimations[animationId];
  dt = static_cast<float>(fmod(dt, scene->mAnimations[animationId]->mDuration));

  std::string nodeName = node->mName.C_Str();
  glm::mat4 currentTransform = glm::mat4(1.0f);

  if (animationMapping.count(std::pair<uint32_t, std::string>(animationId, nodeName)))
  {
    uint32_t channelId = animationMapping[std::pair<uint32_t, std::string>(animationId, nodeName)];
    auto channel = animation->mChannels[channelId];

    // translation matrix
    glm::mat4 translationMatrix =
        InterpolateTranslationMatrix(dt, channel->mPositionKeys, channel->mNumPositionKeys);

    // rotation matrix
    glm::mat4 rotationMatrix =
        InterpolateRotationMatrix(dt, channel->mRotationKeys, channel->mNumRotationKeys);

    // scaling matrix
    glm::mat4 scalingMatrix =
        InterpolateScalingMatrix(dt, channel->mScalingKeys, channel->mNumScalingKeys);

    currentTransform = translationMatrix * rotationMatrix * scalingMatrix;
  }
  else
  {
    currentTransform = CastToGlmMat4(node->mTransformation);
  }

  glm::mat4 globalTransformation = transform * currentTransform;

  // if (boneNamer.Map().count(nodeName))
  // {
  //   uint32_t i = boneNamer.Map()[nodeName];
  //   boneMatrices[i] = globalTransformation * boneOffsets[i];
  // }

  if (boneMapping.count(nodeName))
  {
    uint32_t i = boneMapping[nodeName];
    boneMatrices[i] = globalTransformation * boneOffsets[i];
  }

  for (size_t i = 0; i < node->mNumChildren; i++)
  {
    UpdateBoneMatrices(dt, animationId, node->mChildren[i], globalTransformation);
  }
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

  Workspace& workspace = registry->GetEditorResource<Workspace>();

  std::string directoryWithoutGltf = RemoveLastObjectFromPath(directory, '\\');
  std::string filenameWithoutGltf = directoryWithoutGltf + '/' + std::string(path);

  if (workspace.relativeProjectRoot->path != "")
  {
    fs::path relative = workspace.relativeProjectRoot->path;
    fs::path relativeFile = relative / fs::path(filenameWithoutGltf);
    filename = ConvertFsToNativePaths(relativeFile.string());
  }
  else
  {
    filename = ConvertFsToNativePaths(filenameWithoutGltf);
  }

  uint32_t textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  // unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
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
    std::cout << "(Model::TextureFromFile) : Texture failed to load at path: " << filename
              << std::endl;
    // std::cout << "(Model::TextureFromFile) : Texture failed to load at path:::: "
    //           << relativeFileString << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}

glm::mat4 Model::InterpolateTranslationMatrix(float dt, aiVectorKey* keys, uint32_t n)
{
  if (n == 0)
  {
    return glm::mat4(1.0f);
  }

  if (n == 1)
  {
    return CastToGlmMat4(keys->mValue);
  }

  if (dt <= keys[0].mTime)
  {
    return CastToGlmMat4(keys[0].mValue);
  }

  if (keys[n - 1].mTime <= dt)
  {
    return CastToGlmMat4(keys[n - 1].mValue);
  }

  aiVectorKey anchor;
  anchor.mTime = dt;

  auto rightPtr = std::upper_bound(
      keys, keys + n, anchor,
      [](const aiVectorKey& a, const aiVectorKey& b) { return a.mTime < b.mTime; });
  auto leftPtr = rightPtr - 1;

  float factor = static_cast<float>((dt - leftPtr->mTime) / (rightPtr->mTime - leftPtr->mTime));
  return CastToGlmMat4(leftPtr->mValue * (1.0f - factor) + rightPtr->mValue * factor);
}

glm::mat4 Model::InterpolateRotationMatrix(float dt, aiQuatKey* keys, uint32_t n)
{
  if (n == 0)
  {
    return glm::mat4(1.0f);
  }

  if (n == 1)
  {
    return CastToGlmMat4(keys->mValue);
  }

  if (dt <= keys[0].mTime)
  {
    return CastToGlmMat4(keys[0].mValue);
  }

  if (keys[n - 1].mTime <= dt)
  {
    return CastToGlmMat4(keys[n - 1].mValue);
  }

  aiQuatKey anchor;
  anchor.mTime = dt;

  auto rightPtr =
      std::upper_bound(keys, keys + n, anchor,
                       [](const aiQuatKey& a, const aiQuatKey& b) { return a.mTime < b.mTime; });
  auto leftPtr = rightPtr - 1;

  float factor = static_cast<float>((dt - leftPtr->mTime) / (rightPtr->mTime - leftPtr->mTime));

  aiQuaternion out;
  aiQuaternion::Interpolate(out, leftPtr->mValue, rightPtr->mValue, factor);
  return CastToGlmMat4(out);
}

glm::mat4 Model::InterpolateScalingMatrix(float dt, aiVectorKey* keys, uint32_t n)
{
  if (n == 0)
  {
    return glm::mat4(1.0f);
  }

  if (n == 1)
  {
    return CastToGlmMat4Scale(keys->mValue);
  }

  if (dt <= keys[0].mTime)
  {
    return CastToGlmMat4Scale(keys[0].mValue);
  }

  if (keys[n - 1].mTime <= dt)
  {
    return CastToGlmMat4Scale(keys[n - 1].mValue);
  }

  aiVectorKey anchor;
  anchor.mTime = dt;

  auto rightPtr = std::upper_bound(
      keys, keys + n, anchor,
      [](const aiVectorKey& a, const aiVectorKey& b) { return a.mTime < b.mTime; });
  auto leftPtr = rightPtr - 1;

  float factor = static_cast<float>((dt - leftPtr->mTime) / (rightPtr->mTime - leftPtr->mTime));
  return CastToGlmMat4Scale(leftPtr->mValue * (1.0f - factor) + rightPtr->mValue * factor);
}
