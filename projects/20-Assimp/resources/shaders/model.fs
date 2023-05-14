#version 330 core

struct Material {
   sampler2D texture_diffuse1;
   sampler2D texture_specular1;
   float shininess;
};

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform Material material;

out vec4 FragColor;

void main()
{
    FragColor = vec4(texture(material.texture_diffuse1, TexCoord).rgb, 1.0);
}
