#version 330 core
struct Light {
   vec3 position;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

struct Material {
   sampler2D diffuse;
   vec3 specular;
   float shininess;
};

uniform Light light;
uniform Material material;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

// viewPos is here needed only because we compute the lighting in world space and not viewspace
// We can actually compute in view space and not need viewPos here because viewPos is always (0,0,0) in that case.
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
   // Ambient light
   vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

   // Diffuse light
   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(light.position - FragPos);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

   // Specular light
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = light.specular * (spec * material.specular);

   vec3 result = ambient + diffuse + specular;

   FragColor = vec4(result, 1.0);
}
