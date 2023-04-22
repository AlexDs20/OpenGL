#version 330 core
struct Light {
   vec3 direction;

   vec3 position;
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   float constant;
   float linear;
   float quadratic;
};

struct Material {
   sampler2D diffuse;
   sampler2D specular;
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
   // Compute attenuation of light due to distance
   float distance = length(light.position - FragPos);
   float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

   // Ambient light
   vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

   // Diffuse light
   vec3 norm = normalize(Normal);
   vec3 lightDir;
   if (false)   // if we work with the directional light
      lightDir = normalize(-light.direction);
   else       // if we work with the position of the light
      lightDir =  normalize(light.position - FragPos);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;

   // Specular light
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;

   FragColor = vec4(attenuation * (ambient + diffuse + specular), 1.0);
}
