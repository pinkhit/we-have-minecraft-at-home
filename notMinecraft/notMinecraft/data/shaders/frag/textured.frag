#version 410 core
out vec4 FragColor;

in vec2 uv;

uniform sampler2D ourTexture;

void main()
{
  FragColor = texture(ourTexture,uv);
}