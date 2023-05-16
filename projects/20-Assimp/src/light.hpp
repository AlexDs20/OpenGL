#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm/glm.hpp>

struct PointLight {
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant = 1.0f;
    float linear;
    float quadratic;
};

#endif
