#ifndef MESH_HPP
#define MESH_HPP

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "shader.hpp"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;   // diffuse, specular
    std::string path;
    float shininess;
};

class Mesh {
    public:
        // mesh data
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

        void Draw(Shader &shader);

    private:
        unsigned int VAO, VBO, EBO;

        void setupMesh();
};

#endif