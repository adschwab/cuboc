#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 vTexCoords;

out vec2 texCoords;

void main() {
  texCoords = vTexCoords;

  gl_Position = vec4(position, 0.0, 1.0);
}

