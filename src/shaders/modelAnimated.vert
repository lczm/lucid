#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in ivec4 boneIDs;
layout(location = 4) in vec4 weights;

out vec2 TexCoords;

const int MAX_BONES = 100;
uniform mat4 gBones[MAX_BONES];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  TexCoords = aTexCoords;

  // mat4 boneTransform = gBones[boneIDs[0]] * weights[0];
  // boneTransform += gBones[boneIDs[1]] * weights[1];
  // boneTransform += gBones[boneIDs[2]] * weights[2];
  // boneTransform += gBones[boneIDs[3]] * weights[3];

  mat4 boneTransform = gBones[0] * weights[0];
  boneTransform += gBones[0] * weights[1];
  boneTransform += gBones[0] * weights[2];
  boneTransform += gBones[0] * weights[3];

  vec4 position = boneTransform * vec4(aPos, 1.0);
  gl_Position = projection * view * model * position;
}