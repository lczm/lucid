#version 330 core

const int MAX_BONES = 100;

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in ivec4 boneIds;
layout(location = 4) in vec4 weights;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 boneMatrices[MAX_BONES];

void main()
{
  TexCoords = aTexCoords;

  mat4 boneTransform = mat4(1.0);
  boneTransform += boneMatrices[boneIds[0]] * weights[0];
  boneTransform += boneMatrices[boneIds[1]] * weights[1];
  boneTransform += boneMatrices[boneIds[2]] * weights[2];
  boneTransform += boneMatrices[boneIds[3]] * weights[3];

  // mat4 boneTransform = mat4(0.0);
  // for (int i = 0; i < 4; i++)
  // {
  //   boneTransform += boneMatrices[boneIds[i]] * weights[i];
  // }

  vec4 position = boneTransform * vec4(aPos, 1.0);
  gl_Position = projection * view * model * position;
}