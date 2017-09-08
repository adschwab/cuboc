#version 330 core
out vec4 color;

in vec2 texCoords;

uniform sampler2D texture1;

void main()
{
  color = texture(texture1, texCoords);
}

