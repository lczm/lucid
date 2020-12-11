#version 330 core

layout(location = 0) in vec3 instanceColor;
layout(location = 1) in mat4 instanceMatrix;

out vec3 color;

uniform mat4 view;
uniform mat4 projection;

void main()
{
  color = instanceColor;

  vec3 aPos = gl_VertexID == 0 ? vec3(0.0, 0.0, 0.0) : vec3(1.0, 1.0, 1.0);
  gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);
}