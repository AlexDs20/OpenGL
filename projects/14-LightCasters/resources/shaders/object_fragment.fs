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

   // if spotlight
   float lowCutOff;
   float highCutOff;
};

struct Material {
   sampler2D diffuse;
   sampler2D specular;
   float shininess;
};

uniform Light light;
uniform Light flashLight;
uniform Material material;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

// viewPos is here needed only because we compute the lighting in world space and not viewspace
// We can actually compute in view space and not need viewPos here because viewPos is always (0,0,0) in that case.
uniform vec3 viewPos;

out vec4 FragColor;


vec3 compute_light(Light light, vec3 fragPos, vec3 normal, vec2 texCoord, Material mat, vec3 viewPos){
   // Compute attenuation of light due to distance
   float distance = length(light.position - fragPos);
   float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

   // Diffuse light
   vec3 norm = normalize(normal);
   vec3 lightDir;
   if (false)   // if we work with the directional light
      lightDir = normalize(-light.direction);
   else if (true)      // if we work with the position of the light
      lightDir =  normalize(light.position - fragPos);

   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = light.diffuse * diff * texture(mat.diffuse, texCoord).rgb;

   // Specular light
   vec3 viewDir = normalize(viewPos - fragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
   vec3 specular = light.specular * spec * texture(mat.specular, texCoord).rgb;

   return attenuation * (diffuse + specular);
};

void main()
{
   // Ambient light
   vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;


   vec3 light1 = compute_light(light, FragPos, Normal, TexCoord, material, viewPos);

   // Same for flash light
   vec3 lightDir;
   if (false)   // if we work with the directional light
      lightDir = normalize(-flashLight.direction);
   else if (true)      // if we work with the position of the light
      lightDir =  normalize(flashLight.position - FragPos);
   float cosTheta = dot(lightDir, normalize(-flashLight.direction));

   vec3 light2 ;
   if (cosTheta > flashLight.lowCutOff) {
      light2 = compute_light(flashLight, FragPos, Normal, TexCoord, material, viewPos);
   } else {
      light2 = vec3(0.0f, 0.0f, 0.0f);
   }

   float intensity = clamp(
            (cosTheta - flashLight.highCutOff) / (flashLight.lowCutOff - flashLight.highCutOff),
            0.0f,
            1.0f);


   FragColor = vec4(light1 + intensity * light2 + ambient, 1.0);
}
