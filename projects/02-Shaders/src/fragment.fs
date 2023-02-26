#version 330 core

in vec3 vertexColor;

uniform float ourColor;

out vec4 FragColor;

void main()
{
   FragColor = ourColor * vec4(vertexColor, 1.0);
}
