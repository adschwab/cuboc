#version 330 core
out vec4 color;

in vec2 texCoords;

uniform vec3 fogColor;
uniform sampler2D texture1;

void main()
{
  color = texture(texture1, texCoords);
  float depth = gl_FragCoord.z / gl_FragCoord.w;
  float fogFactor = smoothstep(1.0, 10.0, depth);
  color.rgb = mix(color.rgb, fogColor, fogFactor);
}

