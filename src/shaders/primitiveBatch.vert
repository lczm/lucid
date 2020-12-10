#version 330 core

// layout(location = 0) in vec3 aPos;
layout(location = 0) in mat4 instanceMatrix;
layout(location = 4) in vec3 instanceColor;

out vec3 color;

// uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
// uniform vec3 uColor;

void main() {
  // color = vec3(0.5, 0.5, 0.5);
  color = instanceColor;

  vec3 aPos = gl_VertexID == 0 ? vec3(0.0, 0.0, 0.0) : vec3(1.0, 1.0, 1.0);
  gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);
}