#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 vTexCoords;

out vec2 texCoords;

void main() {
  texCoords = vTexCoords;
  gl_Position = vec4(position.x, position.y, position.z, 1.0);
}

