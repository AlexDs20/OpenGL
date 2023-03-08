#version 330 core

in vec3 vertexColor;
in vec2 TexCoord;

uniform float ourColor;
uniform sampler2D texture1;

out vec4 FragColor;

void main()
{
   FragColor = texture(texture1, TexCoord) + ourColor * vec4(vertexColor, 1.0);
}
