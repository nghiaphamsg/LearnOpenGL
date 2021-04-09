#version 450 core
out vec4 FragColor;

in vec2 TexCoord;

// Texture sampler
uniform sampler2D texture;

void main()
{
    FragColor = texture(texture, TexCoord);
}