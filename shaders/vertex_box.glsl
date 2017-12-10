#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 vTexCoords;

out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float scale;

void main() {
  texCoords = vTexCoords;
  mat4 scale_m = mat4(scale);
  scale_m[3][3] = 1.0;

  scale_m[3][0] = -0.5 * scale;
  scale_m[3][1] = 0.5 * scale;
  scale_m[3][2] = -0.5 * scale;

  gl_Position = projection * view * model * scale_m * vec4(position, 1.0);
}

