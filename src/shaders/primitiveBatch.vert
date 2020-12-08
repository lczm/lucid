#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in mat4 instanceMatrix;

out vec3 color;

// uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
// uniform vec3 uColor;

void main() {
  color = vec3(0.5, 0.5, 0.5);
  gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);
}