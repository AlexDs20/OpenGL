#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.hpp"
#include "shader.hpp"

unsigned int TextureFromFile(const char *path, const std::string &directory);

class Model{
    public:
        Model(const char *path);
        void Draw(Shader &shader);

    private:
        std::vector<Mesh> m_meshes;
        std::string m_directory;

        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif
