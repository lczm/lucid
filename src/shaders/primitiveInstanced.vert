#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 instanceColor;
layout(location = 2) in mat4 instanceMatrix;

out vec3 color;

uniform mat4 view;
uniform mat4 projection;

void main()
{
  color = instanceColor;

  gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);
}