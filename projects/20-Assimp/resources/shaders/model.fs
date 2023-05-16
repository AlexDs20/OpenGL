#version 330 core

struct Material {
   sampler2D texture_diffuse1;
   sampler2D texture_specular1;
   float shininess1;
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

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

// Material
uniform Material material;

// Lights
#define NR_POINT_LIGHTS 1
uniform PointLight pointLights[NR_POINT_LIGHTS];

// Observer
uniform vec3 viewPos;

out vec4 FragColor;


// functions
vec3 ComputePointLight(PointLight, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);        // This is "from frag to position" convention

    vec3 result = vec3(0.0);
    for (int i = 0; i != NR_POINT_LIGHTS; ++i) {
        result += ComputePointLight(pointLights[i], norm, FragPos, viewDir);
    }

    FragColor = vec4(result, 1.0);
}

vec3 ComputePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    // attenuation due to distance
    float distance = length(fragPos - light.position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    // ambient
    vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoord).rgb;

    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = diff * light.diffuse * texture(material.texture_diffuse1, TexCoord).rgb;

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1.0); // change 1.0 for material.shininess1
    vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoord).xxx;

    return attenuation* (ambient + diffuse + specular);
}
