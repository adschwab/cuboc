#version 330 core
out vec4 color;

in vec2 texCoords;

uniform float incolor1;
uniform float incolor2;

void main()
{
  color = vec4(0.0, incolor1, incolor2, 0.0);
}

