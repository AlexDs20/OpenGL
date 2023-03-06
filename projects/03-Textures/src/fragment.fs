#version 330 core

in vec3 vertexColor;
in vec2 TexCoord;

uniform float ourColor;
uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 FragColor;

void main()
{
   FragColor = mix(
         texture(texture1, TexCoord),
         texture(texture2, vec2(-TexCoord.x, TexCoord.y)),
         0.5
         )
      + ourColor * vec4(vertexColor, 1.0);
}
