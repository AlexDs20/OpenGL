#version 330 core

// Lights
struct DirLight {
   vec3 direction;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

struct PointLight {
   vec3 position;

   float constant;
   float linear;
   float quadratic;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

struct SpotLight {
   vec3 position;
   vec3 direction;

   float constant;
   float linear;
   float quadratic;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   float lowCutOff;
   float highCutOff;
};

struct Material {
   sampler2D diffuse;
   sampler2D specular;
   float shininess;
};

uniform DirLight dirLight;

#define NR_POINT_LIGHTS 1
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform SpotLight spotLight;

uniform Material material;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

// viewPos is here needed only because we compute the lighting in world space and not viewspace
// We can actually compute in view space and not need viewPos here because viewPos is always (0,0,0) in that case.
uniform vec3 viewPos;

out vec4 FragColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
   vec3 norm = normalize(Normal);
   vec3 viewDir = normalize(viewPos - FragPos);

   // directional light
   vec3 result = CalcDirLight(dirLight, norm, viewDir);

   // point lights
   for (int i = 0; i != NR_POINT_LIGHTS; ++i)
      result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

   result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

   FragColor = vec4(result, 1.0);
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
   vec3 lightDir = normalize(-light.direction);

   // ambient
   vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

   // Diffuse light
   float diff = max(dot(normal, lightDir), 0.0);
   vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;

   // Specular light
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;

   return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
   // attenuation of light due to distance
   float distance = length(light.position - fragPos);
   float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

   // ambient
   vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

   vec3 lightDir =  normalize(light.position - fragPos);

   // Diffuse light
   float diff = max(dot(normal, lightDir), 0.0);
   vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;

   // Specular light
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;

   return attenuation * (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
   vec3 lightDir =  normalize(light.position - fragPos);

   float cosTheta = dot(lightDir, normalize(-light.direction));
   float intensity = clamp( (cosTheta - light.highCutOff) / (light.lowCutOff - light.highCutOff), 0.0f, 1.0f);

   float distance = length(light.position - fragPos);
   float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

   // ambient
   vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;

   // Diffuse light
   float diff = max(dot(normal, lightDir), 0.0);
   vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;

   // Specular light
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;

   return attenuation * intensity * (ambient + diffuse + specular);
}
